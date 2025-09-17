#include <algorithm>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

template<typename T>
void printVector(const std::string name, const std::vector<T>& vectorArray)
{
    std::cout << name << std::endl;
    for (const auto& instance : vectorArray)
        std::cout << instance << std::endl;
    std::cout << std::endl;
}

int main() {
    // gebruik functies uit <algorithm> en <functional> om de volgende opdrachten uit te voeren:

    // splits de vector in 2 nieuwe vectoren:
    // 1 met alles wat alfabetisch voor 'purple' komt,
    // 1 met alles er na
    {
        std::vector<std::string> colours{"red", "green", "white", "blue", "orange", "green", "orange", "black", "purple"};

        std::ranges::sort(colours);

        std::vector<std::string> before, after;
        std::partition_copy(colours.begin(), colours.end(),
            std::back_inserter(before),
            std::back_inserter(after),
            [](const std::string &str){ return str < "purple"; });

        printVector("Before:", before);
        printVector("After:", after);
    }

    // maak alle elementen UPPERCASE
    {
        std::vector<std::string> colours{"red", "green", "white", "blue", "orange", "green", "orange", "black", "purple"};

        for (auto it = colours.begin(); it != colours.end(); ++it) {
            auto colour = *it;
            std::ranges::transform(colour, colour.begin(),
                [](unsigned char c) { return c & ~32; });
            *it = colour;
        }

        printVector("UPPERCASE:", colours);
    }

    // verwijder alle dubbele elementen
    {
        std::vector<std::string> colours{"red", "green", "white", "blue", "orange", "green", "orange", "black", "purple"};

        std::ranges::sort(colours);
        colours.erase(std::unique(colours.begin(), colours.end()), colours.end());

        printVector("No duplicates:", colours);
    }

    // verwijder alle negatieve elementen
    {
        std::vector<double> numbers{10, 324422, 6, -23, 234.5, 654.1, 3.1242, -9.23, 635};

        std::erase_if(numbers,
            [](const double number) { return number < 0; });

        printVector("no negatives:", numbers);
    }

    // bepaal voor alle elementen of ze even of oneven zijn
    {
        std::vector numbers{10, 324422, 6, -23, 234, 654, 3, -9, 635};

        // This would most likely be better in a simple for loop,
        // but I wanted to use the algorithm and template functions
        std::cout << "Odd and even numbers:" << std::endl;
        std::ranges::transform(numbers, numbers.begin(),
            [](const int number) {
                if (number % 2 == 0) {
                    std::cout << number << " = even" << std::endl;
                    return number;
                }
                std::cout << number << " = odd" << std::endl;
                return number;
        });
        std::cout << std::endl;
    }

    // bepaal de som, het gemiddelde, en het product van alle getallen te berekenen
    {
        std::vector<long double> numbers{10, 324422.1, 6, -23, 234.5, 654.1, 3.1242, -9.23, 635};
        // turned it into a long double, hope you won't mind

        std::cout << "Som:" << std::endl;
        const auto som = std::accumulate(numbers.begin(), numbers.end(), 0.0L);
        std::cout << som << std::endl << std::endl;

        std::cout << "Average:" << std::endl;
        const auto average = som / numbers.size();
        std::cout << average << std::endl << std::endl;

        std::cout << "Product:" << std::endl;
        const auto product = std::accumulate(numbers.begin(), numbers.end(), 1.0L, std::multiplies<long double>());
        std::cout << product << std::endl << std::endl;
    }

    return 0;
}
