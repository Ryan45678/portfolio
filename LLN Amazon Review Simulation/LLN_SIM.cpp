/*****************************************
This program simulates Amazon reviews for a product
given a set of probabilities for 1-5 star ratings. If
possible, these probabilities could be derived from
defect rates for known problems in the product being
simulated. The results are written to a text file that
keeps track of how many reviews of each rating (1-5)
have been given. As the number of total reviews grows,
the reviews should converge to the given probabilities
for each rating.

File: LLN_SIM.cpp
Author: Ryan Askew
Date: 2/19/17
******************************************/

# include <iostream>
# include <fstream>
# include <random>
#include <chrono>
using namespace std;

// seed for the random generator based on the current time
unsigned seed = chrono::system_clock::now().time_since_epoch().count();
// create and seed the generator
default_random_engine generator(seed);
//create a distribution from 0.0 to 1.0
uniform_real_distribution<double> distribution(0.0, 1.0);
const int NUM_RATINGS = 5; // number of rating options
// number of simulations to perform
int NUM_SIMS = 1000;
// keeps track of the experimental probability for each rating option for each
// run of the simulation (the relative frequencies for each run)
struct starProb
{
    float experimentalProbs[5] = {0, 0, 0, 0, 0};
};

void setRatingProbabilities(float[]);
void setNumSims();
void runSim(starProb *, int[], float[]);
int generateRandom(float[]);
void writeToFile(starProb*, float[], int[]);

int main()
{

    int reviews[5] = {0, 0, 0, 0, 0}; // fixed array with number of reviews
                                    // for each type
    // stores the given empirical probabilities for each rating type
    float empiricalProbs[5];
    // pointer for a dynamic array storing relative frequencies for each run
    starProb *dataPoints;

    // set the probabilities of each rating
    setRatingProbabilities(empiricalProbs);
    // set the number of simulations to run
    setNumSims();
    // allocate an array to store the relative frequencies
    dataPoints = new starProb[NUM_SIMS];
    // run the simulation
    runSim(dataPoints, reviews, empiricalProbs);
    // write the results to the .txt files
    writeToFile(dataPoints, empiricalProbs, reviews);

    return 0;
}

void setRatingProbabilities(float empiricalProbs[])
{
    char choice = 'y'; // default safe value for choice is yes

    // decide whether to use default probabilities or input custom ones
    cout << "Use default rating probabilities(0.02, 0.06, 0.09, 0.18, 0.65)?"
         << "(y/n): ";
    cin >> choice;

    // choice validation
    while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N')
    {
        cout << "Please enter (y/n): ";
        cin >> choice;
    }

    // set probabilities to default to
    empiricalProbs[0] = 0.02;
    empiricalProbs[1] = 0.06;
    empiricalProbs[2] = 0.09;
    empiricalProbs[3] = 0.18;
    empiricalProbs[4] = 0.65;

    if (choice == 'y' || choice == 'Y')
    {
        // do nothing if defaults selected
        return;
    }
    // otherwise, enter probabilities
    if (choice == 'n' || choice == 'N')
    {

        cout << "Enter 5 probabilities, starting with 1-star and ending with\n"
             << "5-star reviews. These must add up to 1.0 "
             << "(example: 0.2 0.2 0.3 0.1 0.2 ): ";

        cin >> empiricalProbs[0] >> empiricalProbs[1] >> empiricalProbs[2]
            >> empiricalProbs[3] >> empiricalProbs[4];

        // check for valid probabilities
        while ((empiricalProbs[0] + empiricalProbs[1] + empiricalProbs[2]
            + empiricalProbs[3] + empiricalProbs[4]) <= 0.99999||
            (empiricalProbs[0] + empiricalProbs[1] + empiricalProbs[2]
            + empiricalProbs[3] + empiricalProbs[4]) >= 1.00001)

        {
            cout << "Probabilities do not add up to 1.0. Try again: ";
            cin >> empiricalProbs[0] >> empiricalProbs[1] >> empiricalProbs[2]
            >> empiricalProbs[3] >> empiricalProbs[4];
        }
    }

    return;
}

void setNumSims()
{
    char choice = 'y'; // default safe value for choice is yes

    // decide whether to use number of sims or input a new one
    cout << "Use default number of simulations(1000)?"
         << "(y/n): ";
    cin >> choice;

    // choice validation
    while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N')
    {
        cout << "Please enter (y/n): ";
        cin >> choice;
    }

    // use default number of sims
    if (choice == 'y' || choice == 'Y')
    {
        return;
    }

    // enter custom number of sims
    if (choice == 'n' || choice == 'N')
    {

        cout << "Enter the number of simulations to perform"
             << "(must be at least 1): ";

        cin >> NUM_SIMS;

        // validate input / warn for large numbers of sims
        while (NUM_SIMS < 1 || NUM_SIMS >= 100000)
        {
            while (NUM_SIMS < 1)
            {
                cout << "The number of simulations must be at least 1."
                     << "Try again: ";
                cin >>  NUM_SIMS;
            }
            if (NUM_SIMS >= 100000)
            {
                char go = 'n';
                cout << "WARNING: running 100,000 or more simulations may "
                     << "take a while to write the results to a file.\n"
                     << "Continue anyway? (Y/N): ";
                cin >> go;
                while (go != 'y' && go != 'Y' && go != 'n' && go != 'N')
                {
                    cout << "Please enter (y/n): ";
                    cin >> go;
                }
                if (go == 'n' || go == 'N')
                {
                    cout << "Reverting to default number of simulations.\n";
                    NUM_SIMS = 1000;
                    break;
                }
                if (go == 'y' || go == 'Y')
                {
                    cout << "Continuing with the simulation\n";
                    break;
                }
            }
        }
    }

    return;
}

void runSim(starProb * data, int reviews[], float empiricalProbs[])
{
    cout << "Running simulation...";
    // main loop for each simulation run (each 'review')
    for (int i = 0; i < NUM_SIMS; i++)
    {
        // generate a weighted random rating
        int rating = generateRandom(empiricalProbs);
        // increment the corresponding rating type counter
        reviews[rating]++;
        // find the relative frequencies of each review type for each run
        for(int j = 0; j < 5; j++)
        {
            data[i].experimentalProbs[j] = (static_cast<float>(reviews[j])
                                            / (i + 1));
        }
    }
    cout << "Simulation Complete.\n\n";
    return;
}

int generateRandom(float empiricalProbs[])
{
    // get a random value
    double value = distribution(generator);

    int rating = 0;

    // based on the weights (theoretical probabilities) given to the 5 review
    // types, assign a rating
    if (value <= empiricalProbs[0])
    {
        rating = 0;
    }
    else if (value <= (empiricalProbs[0] + empiricalProbs[1]))
    {
        rating = 1;
    }
    else if (value <= (empiricalProbs[0] + empiricalProbs[1] +
                       empiricalProbs[2]))
    {
        rating = 2;
    }
    else if (value <= (empiricalProbs[0] + empiricalProbs[1] +
                       empiricalProbs[2] + empiricalProbs[3]))
    {
        rating = 3;
    }
    else if (value <= (empiricalProbs[0] + empiricalProbs[1] +
                       empiricalProbs[2] + empiricalProbs[3] +
                       empiricalProbs[4]))
    {
        rating = 4;
    }

    return rating;
}

void writeToFile(starProb *data, float empiricalProbs[], int reviews[])
{
    /** write experimental review probabilities **/
    cout << "Writing experimental review probabilities (this may take"
         << " a while)...";
    ofstream exp;
    exp.open("Experimental_Review_Probabilities.txt");
    for (int i = 0; i < NUM_SIMS; i++)
    {
        exp << i + 1 << " ";
        for (int j = 0; j < 5; j++)
            exp << data[i].experimentalProbs[j] << " ";
        exp << "\n";
    }
    exp.close();
    cout << "done\n";

    /** write empirical review probabilities **/
    cout << "Writing empirical review probabilities...";
    ofstream emp;
    emp.open("Empirical_Review_Probabilities.txt");

    for (int i = 0; i < 5; i++)
        emp << i << " ";

    emp << "\n";

    for (int i = 0; i < 5; i++)
        emp << empiricalProbs[i] << " ";

    emp.close();
    cout << "done\n";

    /** write the distribution of reviews **/
    cout << "writing review distribution...";
    ofstream dist;
    dist.open("Review_Distribution.txt");

    for (int i = 0; i < 5; i++)
        dist << i + 1 << " ";

    dist << "\n";

    for (int i = 0; i < 5; i++)
        dist << reviews[i] << " ";

    dist.close();
    cout << "done\n";
}
