#ifndef SHIPYARD_HPP
#define SHIPYARD_HPP

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <exception>
#include <unordered_map>

#define TESTFUNC(name) {#name, name}

/*
 * An ugly macro hack to get around exception throwing checking.
 * Since, C++ doesn't allow passing of exception type to check for
 * throwing of said exception type, we have to resort to defining
 * a macro that can accept an exception type and instantiate the
 * exception in the catch clause of the try block statement.

 * Further more, C++ does not have a finally or else clause for
 * try blocks and thus we further resort to a manual boolean flag
 * comparision to whether the exception was raised or not.
 */
#define AssertThrows(func, ExceptionType) do {                     \
      bool exception_caught = false;                               \
      try {                                                        \
         func();                                                   \
      } catch (const ExceptionType& e) {                           \
         exception_caught = true;                                  \
      }                                                            \
                                                                   \
      if (!exception_caught)                                       \
         throw sy::AssertionError(std::string(#func) +             \
         	" does not throw "  + std::string(#ExceptionType));    \
   } while ( false );

namespace sy {

	inline static std::string red_text(const std::string s)
	{
		std::string res = "\033[1;31m" + s + "\033[0m";
		return res;
	}

	inline static std::string green_text(const std::string s)
	{
		std::string res = "\033[1;32m" + s + "\033[0m";
		return res;
	}

	/*
	 * Main source of raising exception that is caught by
	 * the runner function "run" for detecting and printing
	 * error messages. An additional extra_m field is for
	 * extra message printing for comparing more complex
	 * data structures such as maps or other STL containers.
	 */
	struct AssertionError : public std::exception {
		std::string message;
		std::string extra_m;
		~AssertionError() throw () {}
		AssertionError(std::string msg, std::string extra = "") : message(msg), extra_m(extra) {}
		const char* what() const throw() { return message.c_str(); }
	};

	inline static void AssertTrue(const bool expr, std::string msg = "")
	{
		if (!expr)
			throw AssertionError("false != true", msg);
	}

	inline static void AssertFalse(const bool expr, std::string msg = "")
	{
		if (expr)
			throw AssertionError("true != false", msg);
	}

	template <typename N>
	inline static typename std::enable_if<
		std::is_arithmetic<N>::value
	>::type Assert(const N n1, const N n2, std::string msg = "")
	{
		if (n1 != n2)
			throw AssertionError(std::to_string(n1) + " != " + std::to_string(n2), msg);
	}

	inline static void Assert(const std::string s1, const std::string s2, std::string msg = "")
	{
		if (s1 != s2)
			throw AssertionError(s1 + " != " + s2, msg);
	}

	template <class Key, class T>
	inline static void Assert(
		std::unordered_map<Key, T>& um1,
		std::unordered_map<Key, T>& um2
	) {
		if (um1.size() != um2.size()) {
			const std::string s1 = std::to_string(um1.size());
			const std::string s2 = std::to_string(um2.size());
			throw AssertionError("unordered_map sizes differ: " + s1 + " != " + s2);
		}
		for (const auto& p : um1) {
			std::stringstream s;
			if (um2.find(p.first) == um2.end()) {
				s << "First key difference: \"" << p.first << "\" not in second unordered map.";
				throw AssertionError(s.str());
			} else if (um2[p.first] != p.second) {
				s << "First value difference at key \"" << p.first << "\": " << um2[p.first] << " != " << p.second;
				throw AssertionError(s.str());
			}
			s.str(std::string()); // Clear stringstream.
		}
	}

	template <class Key, class T>
	inline static void Assert(
		std::map<Key, T>& m1,
		std::map<Key, T>& m2
	) {
		if (m1.size() != m2.size()) {
			const std::string s1 = std::to_string(m1.size());
			const std::string s2 = std::to_string(m2.size());
			throw AssertionError("unordered_map sizes differ: " + s1 + " != " + s2);
		}
		for (auto i1 = m1.begin(), i2 = m2.begin();
		     i1 != m1.end() || i2 != m2.end();
		     ++i1, ++i2) {
			std::stringstream s;
			if (i1->first != i2->first) {
				s << "First key difference: \"" << i1->first << "\" not in same position as second map.";
				throw AssertionError(s.str());
			} else if (i1->second != i2->second) {
				s << "First value difference at key \"" << i1->first << "\": " << i1->second << " != " << i2->second;
		     	throw AssertionError(s.str());
			}
			s.str(std::string());
	     }
	}

	inline static void AssertPasses(
		std::pair<std::string, std::function<void(void)>> p
	) {
		try {
			p.second();
		} catch (const std::exception& e) {
			throw AssertionError(p.first + " raised exception (\"" + e.what() + "\"");
		}
	}

	template <class T>
	inline static void Assert(
		const std::vector<T>& v1,
		const std::vector<T>& v2
	) {
		if (v1.size() != v2.size()) {
			const std::string s1 = std::to_string(v1.size());
			const std::string s2 = std::to_string(v2.size());
			throw AssertionError("vector sizes differ: " + s1 + " != " + s2);
		}
		for (uint64_t i = 0; i < v1.size(); ++i) {
			std::string idx = std::to_string(i);
			Assert(v1[i], v2[i], "v1[" + idx + "] != v2[" + idx + "]");
		}
	}

	inline std::unordered_map<std::string, std::function<void(void)>> create_tests(
		 const std::unordered_map<std::string, std::function<void(void)>>& tests
	) {
		return tests;
	}

	inline static void run(
		const std::unordered_map<std::string, std::function<void(void)>>& tests,
		bool verbose = false
	) {

		bool failures = false;
		std::vector<std::string> errors;

		for (const auto& test : tests) {

			bool failed = true;

			if (verbose)
				std::cout << "Running " << test.first << "... ";

			try {
				test.second();
				failed = false;
			} catch (const AssertionError& e) {

				failures = true;
				std::string error = red_text("FAILURE") + " : " + test.first + " ==> " + e.message;

				if (e.extra_m.size() > 0) {
					error += " ; { " + e.extra_m + " }";
				}

				errors.push_back(error);

				std::cout << (verbose ? (red_text("failed") + ".") : "F");
				std::cout << (verbose ? "\n" : "");

			}

			if (!failed)
				std::cout << (verbose ? (green_text("done") + ".\n") : ".");

		}

		if (failures) {
			std::cout << "\n-------------------------------------------\n\n";
			int count = 0;
			for (auto error : errors)
				std::cout << " # " << ++count << " : " << error << std::endl << std::endl;
			exit(-1);
		} else {
			std::cout << (verbose ? "" : "\n");
		}

	}

} // namespace sy

#endif
