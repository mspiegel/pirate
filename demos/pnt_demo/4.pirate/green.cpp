// This file is only needed when pirate-llvm is unavailable.
#ifdef GAPS_DISABLE
int run_green(int argc, char** argv);

int main(int argc, char** argv) {
    return run_green(argc, argv);
}
#endif
