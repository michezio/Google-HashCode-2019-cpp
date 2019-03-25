# Google-HashCode-2019-cpp

This is the algorithm implemented by team SicroMoft for Google HashCode 2019, Qualification Round.

The actual code submitted is the [Python version](https://github.com/michezio/Google-HashCode-2019-python) but this C++ implementation is many times faster and can calculate a solution in minutes instead of hours.

## Results

We used many different versions of the algorithm, this is what we submitted vs the best we managed to get (using any of the versions):

| Case                  |  Submitted         |  Best Score  |
|-----------------------|--------------------|--------------|
| a_example             |  2                 |  2           |
| b_lovely_landscapes   |  226677            |  226677      |
| c_memorable_moments   |  1775              |  1831        |
| d_pet_pictures        |  415869            |  415869      |
| e_shiny_selfies       |  549185            |  560286      |
| **TOTAL**             |**1,193,508**       |**1,204,665** |


### Score Board

Here the ranking at the end of Extended Round

| Group                                            | Rank / Total |
| ------------------------------------------------ | ------------ |
| Italy / Politecnico di Bari (hosted by GDG Bari) | 2 / 5        |
| Italy                                            | 6 / 209      |
| World                                            | 65 / 2924    |

Team name: **SicroMoft**

## Known issues

This code has little improvements made after the end of the round. It sligtly improved our submitted solution even though there might be some bugs since this version increased the score on some of the input but decreased it on others.

Namely, this algorithm gives the score of 1 point on the a_example.txt file instead of the optimal 2 points, which is stupid.

This is due to the fact that this algorithm starts with pictures sorted by crescent tags count. Other variation been tested is starting with randomly ordered pictures, that actually gave us higher scores on some input, but needs to run many times in order to find a high score by luck.

Further tests and optimization need to be done, but since the competition has ended this is left as an exercise for the reader :P

Also, I made a Python script to improve an already found solution. It's very slow though but can increase the score of any solution by some small amount, you can find it in the Python version of this repo.

## Authors
Michele Abruzzese: [@michezio](https://github.com/michezio/)

Riccardo Sormani

Alessandro Romano
