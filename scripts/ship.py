#!/usr/bin/env python3
import os
import re
import datetime
import argparse
import multiprocessing.pool

template = '''/*
	Generated by ship.py on {datetime}.
*/
#include "shipyard.hpp"
{includes}

int main(int argc, char* argv[])
{{
	const auto tests = sy::create_tests({{
{body}
	}});
	sy::run(tests, {verbose});
	return 0;
}}
'''

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='A shipyard framework helper tool.')

    parser.add_argument('-v', '--verbose', action='store_true', help='Verbose output.')
    parser.add_argument(
        '-p', '--pattern', type=str, nargs="?", help='Pattern of tests to include.'
    )
    parser.add_argument(
        '-o', '--output', nargs='?', default='test.cpp', help='Name of output file.'
    )
    parser.add_argument(
        '-d', '--directory', default='.', help='Directory to look for C++ tests in.'
    )

    args = parser.parse_args()

    pattern = re.compile(r'void(\s+)(test_.*)\((\s+|)\)')

    cpp_files = filter(
        lambda file: file[-3:] in ('cpp', 'cxx', 'cc') and file[:5] == 'test_',
        os.listdir(args.directory),
    )

    tests = []
    includes = set()

    def execute(cpp_file):
        file_name = os.path.join(args.directory, cpp_file)
        with open(file_name, 'r') as f:
            text = f.read()
            test_matches = pattern.finditer(text)
            if args.pattern:
                for test in test_matches:
                    test_name = test.groups()[1].strip()
                    if re.match(args.pattern, test_name):
                        includes.add('#include "{}"'.format(cpp_file))
                        tests.append(test_name)
            else:
                includes.add('#include "{}"'.format(cpp_file))
                matches = [match.groups()[1].strip() for match in test_matches]
                tests.extend(matches)

    pool = multiprocessing.pool.ThreadPool(10).imap_unordered(execute, cpp_files)

    for _ in pool:
        pass

    # If no tests were found
    if not tests:
        print('No tests found.')
        exit(1)

    body = '\n'.join(
        sorted(
            list(
                '{tabs}TESTFUNC({test}),'.format(tabs='\t' * 2, test=test)
                for test in tests
            ),
            key=len,
        )
    )
    includes = '\n'.join(sorted(list(includes), key=len))

    out = template.format(
        body=body,
        includes=includes,
        datetime=datetime.datetime.now().ctime(),
        verbose=('true' if args.verbose else 'false'),
    )
    with open(args.output, 'w+') as f:
        f.write(out)
