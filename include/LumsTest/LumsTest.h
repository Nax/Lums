#ifndef LT_INCLUDED_LUMS_TEST_H
#define LT_INCLUDED_LUMS_TEST_H

#include <functional>
#include <cmath>

#define LT_CONCAT(a, b)         a ## b
#define LT_ID(count)            LT_CONCAT(lt_user_indentifier_, count)
#define LT_TEST_SUITE(name)     lt::RegisterTestSuite LT_ID(__COUNTER__)(name, __FILE__, __LINE__); namespace
#define LT_TEST(name)           LT_TEST2(name, __COUNTER__)
#define LT_TEST2(name, count)   static void LT_ID(count)(void); \
                                lt::RegisterTest LT_ID(__COUNTER__)(name, __FILE__, __LINE__, &LT_ID(count)); \
                                static void LT_ID(count)(void)
#define LT_ASSERT(expr)         do { if (lt::assert_helper(#expr, __FILE__, __LINE__, !!(expr))) { return; } } while (0)
#define LT_ASSERT_TRUE(a)       LT_ASSERT(a)
#define LT_ASSERT_FALSE(a)      LT_ASSERT(!(a))
#define LT_ASSERT_EQ(a,b)       LT_ASSERT((a) == (b))
#define LT_ASSERT_NE(a,b)       LT_ASSERT((a) != (b))
#define LT_ASSERT_LT(a,b)       LT_ASSERT((a) < (b))
#define LT_ASSERT_GT(a,b)       LT_ASSERT((a) > (b))
#define LT_ASSERT_LE(a,b)       LT_ASSERT((a) <= (b))
#define LT_ASSERT_GE(a,b)       LT_ASSERT((a) >= (b))
#define LT_ASSERT_EQF(a,b,e)    LT_ASSERT(std::abs((a) - (b)) <= (e))
#define LT_ASSERT_NEF(a,b,e)    LT_ASSERT(std::abs((a) - (b)) > (e))

namespace lt
{

struct RegisterTestSuite { RegisterTestSuite(const char* name, const char* file, int line); };
struct RegisterTest { RegisterTest(const char* name, const char* file, int line, std::function<void(void)> func); };

bool assert_helper(const char* expr, const char* file, int line, bool value);

}

#endif /* LT_INCLUDED_LUMS_TEST_H */
