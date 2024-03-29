# STAR++
<img align="left" src="https://i.imgur.com/RAGGXwJ.png" width=25%>

STAR++ is an implementation of the [STAR voting method](https://www.starvoting.org/).

It exists as a CMake consumable library and reference command-line application that demonstrates use of the library.

The original motivation for this project was to facilitate automated election result calculation of a single-round voting system for the [Philadelphia Film Critics Circle](https://philafcc.org/) annual awards, which was realized as the reference application.

[![Dev Builds](https://github.com/oblivioncth/STARpp/actions/workflows/push-reaction.yml/badge.svg?branch=dev)](https://github.com/oblivioncth/STARpp/actions/workflows/push-reaction.yml)

## Features

 - Reference command-line application for running elections
 - Full implementation of the STAR voting system
 - Supports Bloc STAR Voting (determining the winner(s) for one or more seats)
 - Can be configured to allow true-ties instead of employing a random tiebreak when other tiebreaks have failed
 - Optional extra Condorcet Protocol tiebreaker for Scoring Round ties
 - Optional runoff simulation to reduce unresolvable ties (see [DefactoWinner](https://oblivioncth.github.io/STARpp/class_star_1_1_calculator.html#details))
 - Optional Qt signal connection that details calculation steps

## Library
Detailed documentation of this library, facilitated by Doxygen, is available at: https://oblivioncth.github.io/STARpp/

### Getting Started
Either grab the latest [release](https://github.com/oblivioncth/STARpp/releases/) or [build the library from source](https://oblivioncth.github.io/STARpp/index.html#autotoc_md4), and import using CMake.

Building from source is recommended as this library can easily be integrated as a dependency into your project using CMake's FetchContent. An example of this is demonstrated in the documentation.

Finally, the [Minimal Example](https://oblivioncth.github.io/STARpp/index.html#autotoc_md3) gives a basic overview of the lib's API.

You can also refer to the reference application source to get a better understanding of how to use the library.

## Reference Application
This application creates multiple elections from a specific input format (i.e. an election with multiple categories), calculates the results, and then prints them to stdout. It also will produce a detailed process log in the current working directory.

The structuring is a bit clunky due to limitations with the PFCC's polling method. Once possible, a more friendly format will be devised.

First, it expects a CSV of votes that consists of a header row, followed by one row per ballot. The first two fields of the header row don't matter, but the following fields should consist of the candidates for each category. The first field of a ballot row does not matter, while the second should contain the voter's name (dummy values can be used for anonymity), and finally the remainder should contain a score (0-5) that corresponds to the candidate in the above header row.

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
    
    STARpp <options>

**Options**:
 - **-h | --help | -?:** Prints usage information
 - **-v | --version:** Prints the current version of the tool
 - **-c | --config:** Specifies the path to the category config INI file
 - **-b | --box:** Specifies the path to the ballot box CSV file
 - **-o | --calc-options:** Comma seperated list of calculator options:
    - AllowTrueTies > Ends an election prematurely instead of using a random tiebreaker when an unresolvable tie occurs
    - CondorcetProtocol > Uses the protocol during the scoring round before the random tiebreaker if necessary
    - DefactoWinner > If true ties are enabled and an unresolvable tie occurs for second seed in the qualifier, gives the win to the first seed if they would defeat all of them in the runoff
 - **-m | --minimal:** Only show the results summary

**Example:**

    STARpp -c path/to/cat_config.ini -b path/to/ballot_box.csv
    
Using no calculator options will result in the application following the recommended standard STAR protocol when determining winners.

## Source

### Summary

 - C++20
 - CMake >= 3.23.0
 - Targets:
	 - Windows 10+
	 - Linux

### Dependencies
- Qt6
- [Qx](https://github.com/oblivioncth/Qx/)
- [Neargye's Magic Enum](https://github.com/Neargye/magic_enum)
- [OBCMake](https://github.com/oblivioncth/OBCmake) (build script support, fetched automatically)
- [Doxygen](https://www.doxygen.nl/)  (for documentation)

## Pre-built Releases/Artifacts

Releases and some workflows currently provide builds of STARpp in various combinations of platforms and compilers. View the repository [Actions](https://github.com/oblivioncth/STARpp/actions) or [Releases](https://github.com/oblivioncth/STARpp/releases) to see examples

### Details
The source for this project is managed by a sensible CMake configuration that allows for straightforward compilation and consumption of its target(s), either as a sub-project or as an imported package. All required dependencies except for Qt6 and Doxygen are automatically acquired via CMake's FetchContent mechanism.

The configuration of this projects supports consumption both via find_package() and FetchContent.

See the *Packaging* and *Building From Source* sections of the [documentation](https://oblivioncth.github.io/STARpp/) for a detailed overview of the various CMake options, targets, install components, etc.
