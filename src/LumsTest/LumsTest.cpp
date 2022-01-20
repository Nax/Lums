#include <cstdio>
#include <vector>
#include <LumsTest/LumsTest.h>

namespace
{

struct Label
{
    const char* name;
    const char* file;
    int         line;
};

struct Test
{
    Label labelSuite;
    Label labelTest;

    std::function<void(void)>   func;

    bool errored;
};

struct Context
{
    Label               suiteLabel;
    std::vector<Test>   tests;
    Test*               current;
    int                 errors;
};

Context& getContext(void)
{
    static Context c{};
    return c;
}

void runTest(Test& t)
{
    auto& ctx = getContext();
    ctx.current = &t;
    t.func();
    if (t.errored)
    {
        ctx.errors++;
    }
    else
    {
        std::printf(".");
    }
}

}

lt::RegisterTestSuite::RegisterTestSuite(const char* name, const char* file, int line)
{
    auto& ctx = getContext();
    ctx.suiteLabel = { name, file, line };
}

lt::RegisterTest::RegisterTest(const char* name, const char* file, int line, std::function<void(void)> func)
{
    auto& ctx = getContext();
    Test t;

    t.labelSuite = ctx.suiteLabel;
    t.labelTest = { name, file, line };
    t.func = func;
    t.errored = false;

    ctx.tests.push_back(t);
}

bool lt::assert_helper(const char* expr, const char* file, int line, bool value)
{
    if (!value)
    {
        auto& ctx = getContext();
        ctx.current->errored = true;
        std::printf("x\n");
        std::printf("%s (%s:%d)\n", ctx.current->labelSuite.name, ctx.current->labelSuite.file, ctx.current->labelSuite.line);
        std::printf("%s (%s:%d)\n", ctx.current->labelTest.name, ctx.current->labelTest.file, ctx.current->labelTest.line);
        std::printf("%s (%s:%d)\n", expr, file, line);
        std::printf("\n");
        return true;
    }
    return false;
}

int main(int argc, char** argv)
{
    auto& ctx = getContext();

    int total{};

    for (auto& t : ctx.tests)
    {
        runTest(t);
        total++;
    }
    std::printf("\n\n");

    int errors = ctx.errors;

    std::printf("Ran %d test%s, %d passed, %d error%s.\n", total, (total > 1) ? "s" : "", total - errors, errors, (errors > 1) ? "s" : "");

    return errors ? 1 : 0;
}
