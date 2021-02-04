# german_credit_c
A Logistic regression model to predict likelihood of loan default.
Model was trained on the numerical features of the German Credit Data dataset.
Model coefficients and intercept are saved in weights.json.

Model weights are loaded in the begin function.
expit function (used in prediction) is defined in german_cedit.c ad used in the action frunction to compute probabilities.

input_data.json contains sample records.
input_schema.avsc expects a one-line json (an array of records), or arrays of individual records, each on a new line.
