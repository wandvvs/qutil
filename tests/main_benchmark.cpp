#include <benchmark/benchmark.h>

#include <qutil/io/dump.hpp>
#include <sstream>
#include <vector>

static void BM_DumpVector(benchmark::State& state) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 10, 11};
  std::ostringstream ss;
  for(auto _ : state)
    qutil::io::dump(ss, v);
}
// Register the function as a benchmark
BENCHMARK(BM_DumpVector);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
  std::string x = "hello";
  for(auto _ : state)
    std::string copy(x);
}
BENCHMARK(BM_StringCopy);

BENCHMARK_MAIN();