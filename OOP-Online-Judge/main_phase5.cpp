#include <iostream>
#include <cstdio>

#include "include/VFSManager.h"
#include "include/ProblemBank.h"
#include "include/Submission.h"
#include "include/EvaluationEngine.h"

static bool writeTempSource(const char* path, const char* code) {
    FILE* f = std::fopen(path, "wb");
    if (!f) return false;
    if (!code) code = "";
    int n = 0;
    while (code[n] != '\0') n++;
    std::fwrite(code, 1, n, f);
    std::fclose(f);
    return true;
}

static void printVerdict(const OJ::Verdict& v) {
    std::cout << "Verdict: " << OJ::Verdict::codeToString(v.getCode())
              << " | " << v.getMessage() << "\n";
}

int main() {
    std::cout << "Phase 5: Evaluation Engine demo\n\n";

    // Load a problem from VFS (from Phase 3 seed)
    VFSManager vfs;
    vfs.init("OJ_VOL");

    OJ::ProblemBank bank;
    bank.seedIfMissing(vfs);
    bank.loadFromVFS(vfs);

    OJ::Problem* p = bank.getById(7);
    if (!p) {
        std::cout << "Problem id 7 not found\n";
        return 1;
    }

    // Correct submission (echo integer)
    const char* goodCode =
        "#include <iostream>\n"
        "int main(){ long long x; if(!(std::cin>>x)) return 0; std::cout<<x; return 0; }\n";

    // Wrong submission
    const char* badCode =
        "#include <iostream>\n"
        "int main(){ long long x; if(!(std::cin>>x)) return 0; std::cout<<x+1; return 0; }\n";

    // Write temp sources
    std::system("if not exist \".tmp_eval\" mkdir \".tmp_eval\"");
    writeTempSource(".tmp_eval\\good.cpp", goodCode);
    writeTempSource(".tmp_eval\\bad.cpp", badCode);

    OJ::EvaluationEngine engine(".tmp_eval");

    std::cout << "Evaluating GOOD submission...\n";
    OJ::Submission good(".tmp_eval\\good.cpp");
    printVerdict(engine.evaluate(good, *p));

    std::cout << "\nEvaluating BAD submission...\n";
    OJ::Submission bad(".tmp_eval\\bad.cpp");
    printVerdict(engine.evaluate(bad, *p));

    return 0;
}
