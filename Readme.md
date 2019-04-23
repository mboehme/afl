# Adding predictions to AFL

Link https://www.comp.nus.edu.sg/~mboehme is dead,
and where can one find your paper?
Thanks 


<pre>
                   american fuzzy lop 2.51b (magic_fuzzer)

┌─ process timing ─────────────────────────────────────┬─ overall results ─────┐
│        run time : 0 days, 0 hrs, 10 min, 15 sec      │  cycles done : 6      │
│   last new path : 0 days, 0 hrs, 0 min, 1 sec        │current paths : 3104   │
│ last uniq crash : none seen yet                      │ path coverag : 21.7%  │
│  last uniq hang : none seen yet                      │ uniq crashes : 0      │
│     correctness : 5.486657e-04                       │   uniq hangs : 0      │
│     fuzzability : 4.415707e-01                       │  effec paths : 1.555  │
</pre>

## Overview
Pythia provides *statistical correctness guarantees* for fuzzing campaigns (correctness), and quantifies *how difficult* it is to discover paths in a program (difficulty). Pythia also allows to determine the *progress* of the fuzzing campaign *towards completion* (path coverage) and can *predict* the number of paths discovered at a certain time in the future. Once you reach a "path coverage" of 99%, you can normally abort the fuzzing campaign without expecting too many new discoveries. Once you reach a "correctness" of 1e-8, we expect that it would take about 100 million new executions from the last discovery until the next discovery of a new path / unique crash.

## More details
* My extension (Pythia) allows to **provide statistical correctness guarantees** for fuzzing campaigns (see correctness). For fuzzing campaigns of the same program, fuzzed for the same time using the same seeds, Pythia shows an upper bound on the probability to discover a crashing input when no crashes have been found.
* My extension (Pythia) allows to **quantify just how "difficult" it is to fuzz a program** (see difficulty). This is interesting if you want to improve the fuzzability of your test drivers (e.g., in OSS-Fuzz) or if you want to gauge, how long it might take to fuzz that program to achieve a decent progress / correctness guarantee.
* My extension (Pythia) allows to **determine the progress of the fuzzing campaign towards completion** (see path coverage). Once about 99% of paths have been discovered, you can abort the fuzzing campaign without expecting too many new discoveries. Definitely more reliable than setting AFL_EXIT_WHEN_DONE.
* Of course, we can play with the UI presentation. I could easily add the expected time until discovering the next path / unique crash or other prediction intervals. I could also predict map-coverage expected in the future. Wasn't so sure whether this is so interesting. I could provide 95%-confidence intervals, as well. I could try to "smooth" the predictions and estimates using a moving average to reduce the swings.
* Estimation and prediction is **quite efficient and easily scales**! In the beginning of a fuzzing campaign the estimates and predictions might seem all over the place. However, in the accompanying research article, I show (empirically and theoretically) that the estimator bias reduces and precision increases as the fuzzing campaign continues. You should try it out and see for yourself :)
* Also quite useful to implement a **smart scheduling of fuzzing campaigns**, where each campaign runs long enough to make sufficient progress towards completion but not too long to waste resources. See, some programs are "done" in a few hours while others show good progress for days on end -- Pythia finds that out for you :).

## How to interpret UI
* **current paths**: shows the current number of paths discovered.
* **path coverage**: shows #current paths as a percentage of the *asymptotic* total number of paths discovered. In the beginning when many paths are discovered the estimates are quite strongly biased and might swing a bit. Later, when an asymptote is discernable (see afl-plot), the path coverage estimate becomes slightly positively biased, meaning that it might slightly over-estimate the current path coverage. However, the magnitude of the bias reduces as the fuzzing campaign continues. Moreover, there are better estimators available -- but they are more complex (and less efficient) to compute. I would change this based on your feedback. As you get more familiar with Pythia, the *correctness* and *difficulty* values should allow you to tell how trustworthy the path coverage estimate is.
* **correctness**: shows an upper bound on the probability to expose a unique crash if no crashes have been found. The number is fairly representative of fuzzing campaigns of similar length, assuming AFL is started with the same seeds, parameters, and test driver. 
* **fuzzability**: shows how "difficult" it is to discover new paths in that program. Fuzzability is a program property where programs with fuzzability 0 are extremely difficult to fuzz while programs a very high degree of fuzzability are much easier to fuzz.

## Gotchas
* I did not test Pythia when there are more than one instances (-M / -S). Maybe this needs to be accounted for in the implementation. However, again I expect that estimator performance improves with time (i.e., #cycles). 
* For some programs, the asymptotic total number of paths might be estimated several times higher than the current number of paths even after few days. The estimate is based on the number of paths exercised exactly once (see first number @ prediction info), which in such a case would be relatively high (e.g., 80% of paths are exercised exactly once). Consider this: Even after such a long time most of the discovered paths have still been exercised not more than once. Quite certainly, there are many other paths that have not been exercised at all.

If there is enough interest, I'll put up a technical report explaining the research behind this. Also, let me know if you want to build on this research and cite my paper :) <br/>
The central ideas also work for Libfuzzer, syzcaller, Peach, CSmith, and many other fuzzers (except for fuzzers based on symbolic execution) and other program analysis (not only for path coverage).

Cheers - Marcel
