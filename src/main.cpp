#include <algorithm>
#include <iostream>
#include <vector>


void printStringVector(std::string name, std::vector<std::string>& vectorArray)
{
    std::cout << name << std::endl;
    for (const auto& colour : vectorArray)
        std::cout << colour << std::endl;
    std::cout << std::endl;
}

int main() {
    // gebruik functies uit <algorithm> en <functional> om de volgende opdrachten uit te voeren:

    // splits de vector in 2 nieuwe vectoren:
    // 1 met alles wat alfabetisch voor 'purple' komt,
    // 1 met alles er na
    {
        std::vector<std::string> colours{"red", "green", "white", "blue", "orange", "green", "orange", "black", "purple"};

        // First sort based on alfabet
        std::ranges::sort(colours);

        // Get position of purple
        auto purple = std::find(colours.begin(), colours.end(), "purple");

        // Copy to the two Vectors
        std::vector<std::string> firstVector{};
        std::vector<std::string> secondVector{};

        std::copy(colours.begin(), purple, std::back_inserter(firstVector));
        std::copy(purple, colours.end(), std::back_inserter(secondVector));

        // print for checking
        printStringVector("First:", firstVector);
        printStringVector("Second:", secondVector);
    }

    // maak alle elementen UPPERCASE
    {
        std::vector<std::string> colours{"red", "green", "white", "blue", "orange", "green", "orange", "black", "purple"};

        for (const auto& colour : colours) {
            const std::string firstChar(1, colour[0] & ~32);
            colour.replace(colour.begin(), 1, firstChar);
        }
    }

    // verwijder alle dubbele elementen
    {
        std::vector<std::string> colours{"red", "green", "white", "blue", "orange", "green", "orange", "black", "purple"};
    }

    // verwijder alle negatieve elementen
    {
        std::vector<double> numbers{10, 324422, 6, -23, 234.5, 654.1, 3.1242, -9.23, 635};
    }

    // bepaal voor alle elementen of ze even of oneven zijn
    {
        std::vector numbers{10, 324422, 6, -23, 234, 654, 3, -9, 635};
    }

    // bepaal de som, het gemiddelde, en het product van alle getallen te berekenen
    {
        std::vector<double> numbers{10, 324422.1, 6, -23, 234.5, 654.1, 3.1242, -9.23, 635};
    }

    return 0;
}
