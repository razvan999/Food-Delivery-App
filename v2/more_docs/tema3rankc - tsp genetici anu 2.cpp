#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <random>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <fstream>
#include <string>
#include <cstring>

#define nr_pop 500
#define crossover_prob 0.6
#define nr_mat 48
#define orase nr_mat
#define nr_matf float(nr_mat)
#define mutation_prob 1/nr_matf
//#define mutation_prob 0.001
using namespace std;

typedef vector<int> individ;


float random_intv();
int random_intv_a_b(int a, int b);
int random0_1();

vector<int> creare_individ(int lungime);

void remove(std::vector<int>& v, int index);
vector<int> decodificare_individ(vector<int>& vect, int lungime);

vector<int> slice(vector<int>& vec, int pstart, int pend);


float calculare_drum(vector<int> drum, float mat[nr_mat][nr_mat]);

void initializare(float graf[orase][orase]) {
    for (int i = 0; i < orase; i++)
        for (int j = 0; j < orase; j++)
            graf[i][j] = 0;
}

void citire_fisier(float graf[orase][orase]) {
    char file_name[20];
    strcpy(file_name, "att48.xml");
    ifstream file(file_name);

    int index_i = 0, index_j = 0;

    bool begin_tag = false;
    string line;
    while (getline(file, line)) {
        string tmp;
        for (int i = 0; i < line.size(); i++)
            if (!(line[i] == ' ' && tmp.size() == 0))
                tmp += line[i];

        int j = 0;
        while (true)
            if (tmp.compare(j, 1, " ") == 0)
                j++;
            else
                break;

        if (tmp.compare(j, 8, "<vertex>") == 0) {
            begin_tag = true;
            continue;
        }
        else if (tmp.compare(j, 9, "</vertex>") == 0) {
            begin_tag = false;
            index_i++;
        }

        if (begin_tag) {
            float cost = 0;
            int citesc_cost = 0, index_j = 0;
            char sep[] = "\"<>";
            char* p = strtok(&tmp[0], sep);
            while (p != NULL) {
                if (citesc_cost == 1) {
                    cost = stof(p);
                }

                if (citesc_cost == 2) {
                    index_j = stoi(p);
                }

                citesc_cost++;
                p = strtok(NULL, sep);
            }

            graf[index_i][index_j] = cost;
        }

    }

    file.close();
}

void afisare(float graf[orase][orase]) {
    for (int i = 0; i < orase; i++) {
        for (int j = 0; j < orase; j++)
            cout << graf[i][j] << " , ";
        cout << '\n';
    }
}

//////////////////////////////////////////////////////////

void selection(vector<individ>& population, int nr_oras, float mat[nr_mat][nr_mat]);

void crossover1(individ& p1, individ& p2, individ& c1, individ& c2);

void crossover(vector<individ>& population, int nr_oras, float mat[nr_mat][nr_mat]);

int mutate_indiv(individ& indiv, float mprob);

void mutation(vector<individ>& population);



float genetic_algo(float mat[nr_mat][nr_mat]);


void afisare_vect(vector<int> X)
{
    for (const int& i : X)
    {
        cout << i<<" ";
    }
    cout << endl;
}
int main()
{
    float matricenoua[nr_mat][nr_mat];
    initializare(matricenoua);
    citire_fisier(matricenoua);
    //afisare(matricenoua);

    
    cout << "start" << endl;
	int number_rep = 1;
	float bestrez, worstrez, avgrez;
	int vector[31];
	int index = 0;
	chrono::duration<double> best_elapsed_seconds, worst_elapsed_seconds, avg_elapsed_seconds;
	chrono::time_point<chrono::system_clock>begin1, end1;
	begin1 = chrono::system_clock::now();
	bestrez = genetic_algo(matricenoua);
	avgrez = bestrez;
	worstrez = bestrez;
	vector[index] = bestrez;
	index++;
	end1 = chrono::system_clock::now();

	best_elapsed_seconds = end1 - begin1;
	worst_elapsed_seconds = best_elapsed_seconds;
	avg_elapsed_seconds = best_elapsed_seconds;

	cout << bestrez << endl;

	while (number_rep < 5)
	{

		chrono::time_point<chrono::system_clock>begin, end;
		begin = chrono::system_clock::now();
		float rezultat = genetic_algo(matricenoua);

		vector[index] = rezultat;
		index++;
		end = chrono::system_clock::now();
		chrono::duration<double> elapsed_seconds = end - begin;
		cout << rezultat << endl;
		avgrez = avgrez + rezultat;
		if (bestrez > rezultat)
			bestrez = rezultat;
		if (worstrez < rezultat)
			worstrez = rezultat;

		avg_elapsed_seconds = avg_elapsed_seconds + elapsed_seconds;
		if (best_elapsed_seconds > elapsed_seconds)
			best_elapsed_seconds = elapsed_seconds;
		if (worst_elapsed_seconds < elapsed_seconds)
			worst_elapsed_seconds = elapsed_seconds;

		number_rep++;
	}

	avg_elapsed_seconds = avg_elapsed_seconds / 30;
	avgrez = avgrez / 30;

	cout << fixed << "best " << bestrez << " avg " << avgrez << " worst " << worstrez << endl;
	cout << "best_time " << best_elapsed_seconds.count() << "avg_time " << avg_elapsed_seconds.count() << "worst_time " << worst_elapsed_seconds.count() << endl;
	float deviation = 0;
	for (int i = 0; i < index; i++)
	{
		deviation = deviation + pow(abs(vector[i] - avgrez), 2);
	}
	deviation = deviation / (index - 1);
	cout << fixed << "deviation " << sqrt(deviation) << "\n";

}


float random_intv()
{
    random_device r;
    mt19937 gen(r());
    uniform_real_distribution<> distrib(0, 1);

    return distrib(r);
}

int random_intv_a_b(int a, int b)
{
    random_device r;
    mt19937 gen(r());
    uniform_int_distribution<> distrib(a, b);
    return distrib(r);
}

int random0_1()
{
    random_device r;
    mt19937 gen(r());
    uniform_int_distribution<> distrib(0, 1);
    return distrib(r);
}

vector<int> creare_individ(int lungime)
{
    vector<int> newindiv;
    for (int i = 0; i < lungime ; i++)
    {
        newindiv.push_back(random_intv_a_b(0, lungime - 1 - i));
    }

    return newindiv;
}

void remove(std::vector<int>& v, int index) {
    v.erase(v.begin() + index);
}

vector<int> decodificare_individ(vector<int>& vect, int lungime)
{
    vector<int> copiev, decod, index;
    copiev = vect;

    for (int i = 0; i < lungime; i++)
    {
        index.push_back(i);
    }

    for (int i = 0; i < copiev.size(); i++)
    {
        decod.push_back(index[copiev[i]]);
        remove(index, copiev[i]);
    }
    //decod.push_back(index[0]);

    return decod;
}

vector<int> slice(vector<int>& vec, int pstart, int pend)
{
    auto start = vec.begin() + pstart;
    auto end = vec.begin() + pend + 1;

    vector<int> sliced(pend - pstart + 1);

    copy(start, end, sliced.begin());

    return sliced;
}

///////////////////////////////////////////////////////////////
float calculare_drum(vector<int> drum, float mat[nr_mat][nr_mat])
{
    float sum = 0;
    for (int i = 0; i < drum.size() - 1; i++)
    {
        sum = sum + mat[drum[i]][drum[i + 1]];
    }
    sum = sum + mat[drum[drum.size() - 1]][drum[0]];
    return sum;
}







//////////////////////////////////////selection

void selection_rank(vector<individ>& population, int nr_oras, float mat[nr_mat][nr_mat])
{
    vector<float> E;
    for (int i = 0; i < population.size(); i++)
    {
        float interm = calculare_drum(decodificare_individ(population[i], nr_mat), mat);
        E.push_back(interm);
        //cout << i << "  " << interm << endl;
    }

    vector<double> fitness;
    float total_fit = 0;

    //cout << endl << endl << " before " << endl;
    for (int i = 0; i < population.size(); i++)
    {
        //fitness.push_back((maxE - E[i]) / (maxE - minE + 0.000001) + 0.01);
        fitness.push_back(1 / E[i]);
        total_fit = total_fit + fitness[i];
        //afisare_vect(population[i]);
        //cout<< i << " " << fitness[i] << endl;
    }

    //cout << endl << endl << " after " << endl;
    vector<int> index;
    for (int i = 0; i < population.size(); i++)
    {
        index.push_back(i);
    }
    sort(index.begin(), index.end(), [&](int a, int b) { return fitness[a] < fitness[b]; });

    //calculare rank
    vector<float> rank;
    for (int i = 0; i < population.size(); i++)
    {
        //rank.push_back(2-selectionpressure+(2*(selectionpressure-1)*((i+1-1)/(population.size()-1))));
        float valrank = (i + 1.0) / ((population.size() + 1) * population.size() / 2);
        rank.push_back(valrank);
        //cout << rank[i] << endl;
    }

    vector<float> Q;
    Q.push_back(0);
    for (int i = 0; i < population.size(); i++)
    {
        Q.push_back(Q[i] + rank[i]);
        //cout << Q[i] << " ";
    }

    vector<individ> newpopulation;
    newpopulation.reserve(nr_pop);
    int alegeri = 0;

    //int numar_populatie = 0.1 * nr_pop;


    //selectare prorpiu zis
    int numar_populatie = 0;
    while (numar_populatie < nr_pop)
    {
        float r = random_intv();
        for (int j = 0; j < Q.size() - 1; j++)
        {
            if (r > Q[j] && r <= Q[j + 1])
            {
                alegeri++;
                newpopulation.push_back(population[index[j]]);
                //cout << " probab "<< Q[j] << " a fost ales pop " << index[j] << " codif  ";
                //afisare_vect(population[index[j]]);
                numar_populatie++;
                break;
            }
        }
    }
    population = newpopulation;
}

void selection(vector<individ>& population, int nr_oras, float mat[nr_mat][nr_mat])
{
    float minE;
    float maxE;

    minE = calculare_drum(decodificare_individ(population[0], nr_mat), mat);
    maxE = minE;

    vector<float> E;
    E.push_back(minE);
   // cout << "evals" << endl;
    //cout << " 0 " << minE << endl;
    for (int i = 1; i < population.size(); i++)
    {
        float interm = calculare_drum(decodificare_individ(population[i], nr_mat), mat);
        if (minE > interm) {
            minE = interm;
        }
        if (maxE < interm) {
            maxE = interm;
        }
        E.push_back(interm);

       // cout << i << "  " << interm << endl;
    }


    vector<double> fitness;
    float total_fit = 0;
    //cout << "fitness " << endl;
    for (int i = 0; i < population.size(); i++)
    {
        //fitness.push_back((maxE - E[i]) / (maxE - minE + 0.000001) + 0.01);
        fitness.push_back(1 / E[i]);
        total_fit = total_fit + fitness[i];
        //cout << i << " " << fitness[i] << endl;
    }
    //////////////////////////////////////////////////////////////
    vector<float> probability;
    float sumprobs = 0;
    for (int i = 0; i < population.size(); i++)
    {
        probability.push_back(fitness[i] / total_fit);
        sumprobs = sumprobs + probability[i];
        //cout << i << " fitness: " << fitness[i] << " probs: " << probability[i] << endl;
    }

    vector<float> Q;
    Q.push_back(0);
    for (int i = 0; i < population.size(); i++)
    {
        Q.push_back(Q[i] + probability[i]);
    }

    int alegeri = 0;
    vector<individ> newpopulation;
    newpopulation.reserve(nr_pop);

    vector<int> index;
    for (int i = 0; i < population.size(); i++)
        index.push_back(i);

    sort(index.begin(), index.end(), [&](int a, int b) { return fitness[a] > fitness[b]; });

    for (int i = 0; i < 0.1 * nr_pop; i++)
    {
        newpopulation.push_back(population[index[i]]);
    }

    int numar_populatie = 0.1 * nr_pop;
    while (numar_populatie < nr_pop)
    {
        float r = random_intv();
        for (int j = 0; j < Q.size() - 1; j++)
        {
            if (r > Q[j] && r <= Q[j + 1])
            {

                alegeri++;
                //newpopulation.push_back(population[index[j]]);
                newpopulation.push_back(population[j]);
                numar_populatie++;
                break;
            }
        }
    }
    population = newpopulation;
}

























/////////////////////////////////////cross
void crossover1(individ& p1, individ& p2, individ& c1, individ& c2)
{
    c1.reserve(p1.size());
    c2.reserve(p1.size());

    int lungimea_minima = 1;

    int start = 1;
    int ending = p1.size() - 1;

    int taiere1 = random_intv_a_b(start, ending - lungimea_minima);
    //int taiere1 = 3;
    //cout<<" start "<<start  << "ending este " << ending << " taiere " << taiere1 << endl;
    vector<int> p11 = slice(p1, 0, taiere1 - 1);
    vector<int> p12 = slice(p1, taiere1, p1.size() - 1);

    vector<int> p21 = slice(p2, 0, taiere1 - 1);
    vector<int> p22 = slice(p2, taiere1, p2.size() - 1);

    c1.insert(c1.end(), p11.begin(), p11.end());
    c1.insert(c1.end(), p22.begin(), p22.end());

    c2.insert(c2.end(), p21.begin(), p21.end());
    c2.insert(c2.end(), p12.begin(), p12.end());
}

void crossover2(individ& p1, individ& p2, individ& c1, individ& c2)
{
    c1.reserve(p1.size());
    c2.reserve(p1.size());

    int lungimea_minima = 2;

    int start = 1;
    int ending = p1.size() - 1;

    int taiere1 = random_intv_a_b(start, ending - lungimea_minima);
    int taiere2 = taiere1 + lungimea_minima + random_intv_a_b(0, ending - (taiere1 + lungimea_minima) + 1);



    vector<int> p11 = slice(p1, 0, taiere1 - 1);
    vector<int> p12 = slice(p1, taiere1, taiere2 - 1);
    vector<int> p13 = slice(p1, taiere2, p1.size() - 1);

    vector<int> p21 = slice(p2, 0, taiere1 - 1);
    vector<int> p22 = slice(p2, taiere1, taiere2 - 1);
    vector<int> p23 = slice(p2, taiere2, p1.size() - 1);

    c1.insert(c1.end(), p11.begin(), p11.end());
    c1.insert(c1.end(), p22.begin(), p22.end());
    c1.insert(c1.end(), p13.begin(), p13.end());

    c2.insert(c2.end(), p21.begin(), p21.end());
    c2.insert(c2.end(), p12.begin(), p12.end());
    c2.insert(c2.end(), p23.begin(), p23.end());


}

void crossover(vector<individ>& population, int nr_oras, float mat[nr_mat][nr_mat])
{
    struct probs
    {
        float prob;
        int index;
    };

    vector<probs> population_probs;
    population_probs.reserve(population.size());

    for (int i = 0; i < nr_pop; i++)
    {
        probs p;
        p.index = i;
        p.prob = random_intv();
        population_probs.push_back(p);
    }

    sort(population_probs.begin(), population_probs.end(), [](probs& p1, probs& p2) {
        return p1.prob < p2.prob;
        });

    vector<individ> newpopulation;
    int idx = 0;
    while (idx < population_probs.size() && population_probs[idx].prob <= crossover_prob)
    {
        //cout << " iteratia " << population_probs[index].prob << endl;
        //cout << idx << endl;
        if (population_probs[idx].prob <= crossover_prob && population_probs[idx + 1].prob <= crossover_prob)
        {
            individ p1, p2, c1, c2;
            p1 = population[population_probs[idx].index];
            p2 = population[population_probs[idx + 1].index];
            crossover2(p1, p2, c1, c2);
            newpopulation.push_back(c1);
            newpopulation.push_back(c2);
            //cout<<" amandoua " << index << " " << population_probs[index].prob << " " << population_probs[index + 1].prob << endl;
        }
        else
            if (population_probs[idx].prob <= crossover_prob && population_probs[idx + 1].prob > crossover_prob)
            {
                //selectie 50%
                if (random0_1() == 1)
                {
                    individ p1, p2, c1, c2;
                    p1 = population[population_probs[idx].index];
                    p2 = population[population_probs[idx + 1].index];
                    crossover1(p1, p2, c1, c2);
                    newpopulation.push_back(c1);
                    newpopulation.push_back(c2);
                    //cout << " primul " << index << " " << population_probs[index].prob << " " << population_probs[index + 1].prob << endl;
                }
            }
        idx = idx + 2;
    }

    for (int i = 0; i < newpopulation.size(); i++)
    {
        population.push_back(newpopulation[i]);
    }
}


//////////////////////////////////////////mutation

int mutate_indiv(individ& indiv, float mprob)
{
    int value = 0;
    for (int i = 0; i < indiv.size()-1; i++)
    {
        if (random_intv() < mprob)
        {
            int number= (random_intv_a_b(0, indiv.size() - 1)) % (indiv.size() - i);
           // cout <<" i este "<< i  << " indiv[i] " << indiv[i] << " number " << number << endl;
            while (indiv[i] == number)
            {
                number = (random_intv_a_b(0, indiv.size() - 1)) % (indiv.size() - i);
               // cout << " i este " << i << " indiv[i] " << indiv[i] << " number " << number << endl;
            }
            //cout << " initial " << endl;
            //afisare_vect(indiv);
            indiv[i] = number;
            value = 1;
            //cout << " a fost mutat " << i << " mutatia este " << endl;
            //afisare_vect(indiv);
            //afisare_vect(decodificare_individ(indiv, 5));
            //break;
        }
    }
    return value;
}
void mutation(vector<individ>& population)
{
    vector<individ> newpopulation;

    for (int i = 0; i < population.size(); i++)
    {
        individ a = population[i];
        if (mutate_indiv(a, mutation_prob) == 1)
        {
            //newpopulation.push_back(a);
            population[i] = a;
        }
    }

    // for (int i = 0; i < newpopulation.size(); i++)
    // {
    //     population.push_back(newpopulation[i]);
    // }
}

void mutation_reset(vector<individ>& population)
{
    vector<individ> newpopulation;

    for (int i = 0; i < population.size(); i++)
    {
        individ a = population[i];
        if (mutate_indiv(a, 0.2) == 1)
        {
            //newpopulation.push_back(a);
            population[i] = a;
        }
    }
}
//////////////////////////////////////////////











float best(vector<individ>& population, float mat[nr_mat][nr_mat])
{
    float min = calculare_drum(decodificare_individ(population[0], nr_mat), mat);
    for (int i = 1; i < population.size(); i++)
    {
        float nr = calculare_drum(decodificare_individ(population[i], nr_mat), mat);
        if (nr < min)
        {
            min = nr;
        }
    }
    return min;
}
//////////////////////////////////////////////gen
float genetic_algo(float mat[nr_mat][nr_mat])
{
    vector<individ> population;
    for (int i = 0; i < nr_pop; i++)
    {
        population.push_back(creare_individ(nr_mat));
        //cout << i << ": ";
        //afisare_vect(population[i]);
        //cout << " decodificat: ";
        //afisare_vect(decodificare_individ(population[i], nr_mat));
    }
    //double x=1/nr_matf;
    //cout<< " nr_mat este " <<nr_mat<<" mutation prob " << mutation_prob << " x "<< x << endl;

    /*individ c1, c2;
    crossover2(population[0], population[1], c1, c2);
    afisare_vect(c1);
    afisare_vect(c2);*/
    
    float min = 1000000;
    int cont = 0;
    int cont2 = 0;
    float previous;
    for (int i = 0; i < 2000; i++)
    {
        //selection(population, nr_mat, mat);
        selection_rank(population,nr_mat,mat); 
        crossover(population, nr_mat, mat);
        mutation(population);
        float nr = best(population, mat);

        if (nr < min)
        {
            min = nr;
            cont = 0;
        }
        else
        {
            cont++;
        }

        if(previous==nr && i!=0)
        {
            cont2++;
        }

        if(previous!=nr && i!=0)
        {
            cont2=0;
        }

        if(cont2 % 50 == 0 && i!=0 && cont2!=0)
        {
            mutation_reset(population);
        }        
        previous=nr;


        //cout << " generatia " << i << " nr este " << nr << " best: " << min << " contor este " << cont <<" cont2 "<< cont2 << endl;
    }

    return min;
}