//fastscore.schema.0: input_schema.avsc
//fastscore.slot.1: in-use

#include "fastscore.h"

#include <vector>
#include <list>
#include <math.h>
#include <jansson.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h> 

#include <iostream>
#include <fstream>


// declare global variables for intercept and coefficients of logistic regression
float intercept_value;
std::vector<float> coefficients;


// modelop.init
void begin()
{
	printf("In Begin()\n");

    //load weights (coefficients + intercept) from weights.json
    json_t *weights;
    json_error_t error;
    weights = json_load_file("weights.json", 0, &error);

    // extract intercept from weights file
    json_t *intercept = json_object_get(weights, "intercept");
    intercept_value = json_number_value(intercept);

    // extract coefficient key:values pairs and store values as floats
    json_t *duration_months_coeff = json_object_get(weights, "duration_months");
    json_t *credit_amount_coeff = json_object_get(weights, "credit_amount");
    json_t *installment_rate_coeff = json_object_get(weights, "installment_rate");
    json_t *present_residence_since_coeff = json_object_get(weights, "present_residence_since");
    json_t *age_years_coeff = json_object_get(weights, "age_years");
    json_t *number_existing_credits_coeff = json_object_get(weights, "number_existing_credits");
    json_t *number_people_liable_coeff = json_object_get(weights, "number_people_liable");

    float duration_months_coeff_value = json_number_value(duration_months_coeff);
    float credit_amount_coeff_value = json_number_value(credit_amount_coeff);
    float installment_rate_coeff_value = json_number_value(installment_rate_coeff);
    float present_residence_since_coeff_value = json_number_value(present_residence_since_coeff);
    float age_years_coeff_value = json_number_value(age_years_coeff);
    float number_existing_credits_coeff_value = json_number_value(number_existing_credits_coeff);
    float number_people_liable_coeff_value = json_number_value(number_people_liable_coeff);
    
    // assign coefficeints above to global coefficients vector
    coefficients = {
        duration_months_coeff_value, credit_amount_coeff_value, installment_rate_coeff_value,
        present_residence_since_coeff_value, age_years_coeff_value, 
        number_existing_credits_coeff_value, number_people_liable_coeff_value};

    std::cout << "weights: " << weights << std::endl;
    std::cout << "intercept: " << intercept_value << std::endl;
}


static float dotProduct(std::vector<float> vect_A, std::vector<float> vect_B) { 
    float dot_product = 0; 
    // Loop to calculate dot product 
    for (int i = 0; i < 7; i++) 

        dot_product = dot_product + vect_A[i] * vect_B[i];

    return dot_product; 
} 


static float expit (double x) {
  return 1.0/(1.0+exp(-x));
}


// modelop.score
void action(fastscore_value_t v, int slot, int seqno) {

    printf("In action got value fmt %d slot %d seqno %d\n", v.fmt, slot, seqno );

    int i;
    
    // parse input as json object
    json_t *a=v.js;
    json_t *b=json_array();
    size_t count=json_array_size(a);
    printf("The input array has %d elements\n", count);

    // loop over records in input array to produce predictions
    for (i = 0; i < count; i++)
    {   
        // get record (JSON) from array of records
        json_t *record = json_array_get(a,i);

        // extract key:values pairs and store values as floats
        json_t *duration_months = json_object_get(record, "duration_months");
        json_t *credit_amount = json_object_get(record, "credit_amount");
        json_t *installment_rate = json_object_get(record, "installment_rate");
        json_t *present_residence_since = json_object_get(record, "present_residence_since");
        json_t *age_years = json_object_get(record, "age_years");
        json_t *number_existing_credits = json_object_get(record, "number_existing_credits");
        json_t *number_people_liable = json_object_get(record, "number_people_liable");

        float duration_months_value = json_number_value(duration_months);
        float credit_amount_value = json_number_value(credit_amount);
        float installment_rate_value = json_number_value(installment_rate);
        float present_residence_since_value = json_number_value(present_residence_since);
        float age_years_value = json_number_value(age_years);
        float number_existing_credits_value = json_number_value(number_existing_credits);
        float number_people_liable_value = json_number_value(number_people_liable);

        // Form a vector of input record values
        std::vector<float> input_record = {
            duration_months_value, credit_amount_value, installment_rate_value,
            present_residence_since_value, age_years_value, number_existing_credits_value,
            number_people_liable_value};

        // compute the probablity of class 2
        float pred_probability = expit(dotProduct(coefficients, input_record) + intercept_value);

        // map probability to to classes {1,2}
        int prediction;

        if (pred_probability <= 0.5) {
            prediction = 1;
        } else {
            prediction = 2;
        }

        // declare json object of prediction
	    json_t *response=json_object();

	    json_object_set(response, "prediction", json_real(prediction));
	    
        // append prediction to overall output
        json_array_append_new(b, response );
	    printf( "Just appended to array, current length: %d\n", v, json_array_size(b));
    }
    
    printf("Before fastscore_emit\n");

    // return predictions
    fastscore_emit((fastscore_value_t) {
        .fmt = FASTSCORE_FMT_JSON,
        .js = b,
    }, 1);
    printf("After fastscore_emit, returning from action()\n");
}


void end1()
{
	printf("End1()\n");
}