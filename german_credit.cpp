#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <list>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using json = nlohmann::json;

float dotProduct(std::vector<float> vect_A, std::vector<float> vect_B)
{
    /// @brief function to compute dot product of two 7-d vectors.
    /// @param vect_A first vector
    /// @param vect_B second vector
    /// @return dot product of vec_A and vec_B

    float dot_product = 0;
    // Loop to calculate dot product
    for (int i = 0; i < 7; i++)

        dot_product += vect_A[i] * vect_B[i];

    return dot_product;
}

float expit(float x)
{
    /// @brief expit (inverse of logit) function
    /// @param x real number input
    /// @return expit(x)

    float result = 1.0 / (1.0 + exp(-x));
    return result;
}

void predict(std::string input_file_path)
{

    // variables for intercept and coefficients of logistic regression
    std::vector<float> coefficients;
    float intercept;

    // load weights (coefficients + intercept) from weights.json
    json weights;
    std::ifstream weights_file("weights.json");
    weights_file >> weights;
    weights_file.close();

    for (auto &el : weights.items())
    {
        nlohmann::json weights_object = el.value();

        // extract intercept from weights file
        float intercept = weights_object["intercept"];

        // extract coefficient key:values pairs and store values as floats
        float duration_months_coeff = weights_object["duration_months"];
        float credit_amount_coeff = weights_object["credit_amount"];
        float installment_rate_coeff = weights_object["installment_rate"];
        float present_residence_since_coeff = weights_object["present_residence_since"];
        float age_years_coeff = weights_object["age_years"];
        float number_existing_credits_coeff = weights_object["number_existing_credits"];
        float number_people_liable_coeff = weights_object["number_people_liable"];

        // assign coefficeints above to global coefficients vector
        coefficients = {
            duration_months_coeff, credit_amount_coeff,
            installment_rate_coeff, present_residence_since_coeff,
            age_years_coeff, number_existing_credits_coeff,
            number_people_liable_coeff};
    }

    // Read recods from input JSON array
    json input_records;
    std::ifstream ifs(input_file_path);
    ifs >> input_records;
    ifs.close();

    // Iterate over each record, get values, and form values 7-d vector
    for (auto &element : input_records.items())
    {
        nlohmann::json object = element.value();

        float duration_months = object.value("duration_months", 0);
        float credit_amount = object.value("credit_amount", 0);
        float installment_rate = object.value("installment_rate", 0);
        float present_residence_since = object.value("present_residence_since", 0);
        float age_years = object.value("age_years", 0);
        float number_existing_credits = object.value("number_existing_credits", 0);
        float number_people_liable = object.value("number_people_liable", 0);

        std::vector<float> input_record;
        input_record = {
            duration_months, credit_amount,
            installment_rate, present_residence_since,
            age_years, number_existing_credits,
            number_people_liable};

        // printing input_record for verification
        std::cout << "input_record = { ";
        for (float n : input_record)
        {
            std::cout << n << ", ";
        }
        std::cout << "}; \n";

        // compute the probablity of class 2
        float dot_product = dotProduct(coefficients, input_record);
        float pred_probability = expit(dot_product + intercept);

        // map probability to to classes {1,2}
        int prediction;

        if (pred_probability <= 0.5)
        {
            prediction = 1;
        }
        else
        {
            prediction = 2;
        }

        // printing prediction for verification
        std::cout << "dot_product: " << dot_product << "  pred_probability: " << pred_probability << "  prediction: " << prediction << std::endl
                  << std::endl;
    }
    // printing coefficients for verification
    std::cout << "coefficients = { ";
    for (float n : coefficients)
    {
        std::cout << n << ", ";
    }
    std::cout << "}; \n";
}

int main(int argc, char *argv[])
{
    /// @brief main function to call predict on input data (JSON array file)
    /// @param argc Number of arguments
    /// @param argv JSON Array filename
    /// @return 0 - function write to stdout
    predict(argv[1]);

    std::cout << argv[1] << std::endl;

    return 0;
}