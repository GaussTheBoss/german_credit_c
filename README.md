# german_credit_cpp
A Logistic regression C++ model to predict likelihood of loan default.
Model was trained on the numerical features of the German Credit Data dataset.

### Assets:
 - `input_data.json` contains sample records as an Array of objects.
 - `weights.json` contains logistic regression weights.

### Running locally

Insure you have a gcc compiler, and install the dependency `nlohmann-json3-dev` for your operating system. For Linux, you may use

```
sudo apt-get install nlohmann-json3-dev
```

To compile
```
gcc -o german_credit german_credit.cpp -lm -lstdc++
```

The main function expects a filename input (data to be scored/predicted). To run from command line, use
```
./german_credit "input_data.json"
```

The main function does not write to a file, but rather prints to stdout.
