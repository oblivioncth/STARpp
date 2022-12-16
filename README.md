# StarCalc

StarCalc is an implementation of the [STAR voting method](https://www.starvoting.org/).

It exists as a CMake consumable library and reference command-line application that demonstrates use of the library.

The original motivation for this project was to facilitate automated election result calculation of a single-round voting system for the [Philadelphia Film Critics Circle](https://philafcc.org/) annual awards, which was realized as the reference application.

[![Dev Builds](https://github.com/oblivioncth/StarCalc/actions/workflows/push-reaction.yml/badge.svg?branch=dev)](https://github.com/oblivioncth/StarCalc/actions/workflows/push-reaction.yml)

## Features

 - Reference command-line application for running elections
 - Full implementation of the STAR voting system
 - Supports Bloc STAR Voting (determining the winner(s) for one or more seats)
 - Can be configured to allow true-ties instead of employing a random tiebreak when other tiebreaks have faield
 - Optional extra Condorcet Protocol tiebreaker for Scoring Round ties
 - Optional Qt signal connection that details calculation steps

## Library
The public API of the library is not currently documented (it likely will be in time or given interest), though its usage is fairly straight forward as shown in the following minimal example:

```cpp
#include <star/election.h>
#include <star/calculator.h>
#include <iostream>

int main()
{
    // Setup ballots
    Star::Election::Voter jim{.name = "Jim", .anonymousName = "Voter 1"};
    QList<Star::Election::Vote> jimVotes{
        {.candidate = "Candidate 1", .score = 3},
        {.candidate = "Candidate 2", .score = 1},
        {.candidate = "Candidate 3", .score = 5}
    };

    Star::Election::Voter sarah{.name = "Sarah", .anonymousName = "Voter 2"};
    QList<Star::Election::Vote> sarahVotes{
        {.candidate = "Candidate 1", .score = 4},
        {.candidate = "Candidate 2", .score = 0},
        {.candidate = "Candidate 3", .score = 2}
    };

    Star::Election::Voter ted{.name = "Ted", .anonymousName = "Voter 3"};
    QList<Star::Election::Vote> tedVotes{
        {.candidate = "Candidate 1", .score = 3},
        {.candidate = "Candidate 2", .score = 2},
        {.candidate = "Candidate 3", .score = 1}
    };

    // Setup election
    Star::Election::Builder eb("My Election");
    eb.wBallot(ted, tedVotes)
      .wBallot(sarah, sarahVotes)
      .wBallot(ted, tedVotes);
    eb.wSeatCount(3);

    Star::Election election = eb.build();
    assert(election.isValid());

    // Setup Calculator
    Star::Calculator calculator(&election);

    //If you want calculation details...
    QObject::connect(&calculator, &Star::Calculator::calculationDetail, [](const QString& detail){
        std::cout << detail.toStdString() << std::endl;
    });

    // Determine outcome
    Star::ElectionResult result = calculator.calculateResult();
    assert(!result.isNull());

    std::cout << "Winner(s): " << result.winners().join(", ").toStdString() << std::endl;

    return 0;
}
```
You can also refer to the reference application source to get a better understanding of how to use the library.

## Reference Application
This application creates multiple elections from a specific input format (i.e. an election with multiple categories), calculates the results, and then prints them to stdout.

First, it expects a CSV of votes that consists of a header row, followed by one row per ballot. The first two fields of the header row don't matter, but the following fields should consist of the candidates for each category. The first field of a ballot row does not matter, while the second should contain the candidates name (for now it is unconditionally obfuscated), and finally the remainder should contain a score (0-5) that corresponds to the candidate in the above header row.

**Example:**
```
N/A,Voters,CatACan1,CatACan2,CatACan3,CatBCan1,CatBCan2
X,Jim,0,3,5,4,2
X,Sarah,5,2,2,1,4
X,Ted,3,0,0,5,0
```
Second, it expects an INI file that indicates how many candidates are in the category, in the same order as they are listed in the CSV. These details are to be place under the section "Categories". Additionally, a value for the singular key "Seats" must be specified under the section "General" in order to specify how many candidates are to be elected (Bloc voting). Simply use "1" for a typical election with a single winner.

**Example:**
```
[Categories]
Category A = 3
Category B = 2

[General]
Seats = 2
```
Lastly, the files are loaded via the application's command-line parameters as shown in the following section.

### Usage
The application uses the following syntax scheme:
    
    StarCalc <options>

**Options**:
 - **-h | --help | -?:** Prints usage information
 - **-v | --version:** Prints the current version of the tool
 - **-c | --config:** Specifies the path to the category config INI file
 - **-b | --box:** Specifies the path to the ballot box CSV file
 - **-t | --true-ties:** Ends an election prematurely instead of using a random tiebreaker when an unresolvable tie occurs.
 - **-e | --extra-tiebreak:** Uses the Condorcet protocol tiebreaker during the scoring round before the random tiebreaker if necessary
 - **-m | --minimal:** Only show the results summary

**Example:**

    StarCalc -c path/to/cat_config.ini -b path/to/ballot_box.csv

## Source

### Summary

 - C++20
 - CMake >= 3.24.0
 - Targets:
	 - Windows 10+
	 - Linux (Tested on Ubuntu 20.04)

### Dependencies
- Qt6
- [Qx](https://github.com/oblivioncth/Qx/)
- [Neargye's Magic Enum](https://github.com/Neargye/magic_enum)

### Builds
Tested with:
 - Windows: MSVC2022
 - Linux: Clang 12

### Details
The source for this project is managed by a sensible CMake configuration that allows for straightforward compilation and consumption of its target(s), either as a sub-project or as an imported package. All required dependencies except for Qt6 are automatically acquired via CMake's FetchContent mechanism.

The configuration of this projects supports consumption both via find_package() and FetchContent.

**CMake Options:**

 - `STARCALC_TESTS` set to ON in order to generate tests

**CMake Targets**
 - `all` - Builds the library, reference application, and tests
 - `starcalc_base` - Builds the library
 - `starcalc_frontend` - Builds the reference application
 - `starcalc_tst_...` - Various test targets. To actually run tests, just build the general CMake tests target `test`

**CMake Install Components:**
 - `all` - Installs all built components
 - `starcalc` - Installs top-level files (README.md, CMake package configuration files, etc.)
 - `starcalc_base` - Installs the library
 - `starcalc_frontend` - Installs the reference application
