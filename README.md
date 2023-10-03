## Description

This C++ project generates a set of bash scripts designed to make git commits with randomized dates. The program accepts `start_date`, `end_date`, and the number of commits (`X`) as command-line arguments. It then produces bash files for each commit with dates that lie between Monday to Saturday, and between 7:00 AM and 11:59 PM. Additionally, a `.txt` file containing all the generated dates is created for easy reference.

## Features

- Command-line argument parsing for easy customization.
- Date validation to ensure valid `start_date` and `end_date`.
- Generation of unique random dates within the specified range.
- Bash files are created with git commit commands and the generated dates.
- Bash files auto-delete after successful execution.
- A `.txt` file containing all generated dates is created.

## Prerequisites

- C++ Compiler (e.g., g++, clang)
- CMake (Version >= 3.10)

## Build Instructions

1. Clone the repository to your local machine.
   ```
   git clone https://github.com/yourusername/BashGenerator.git
   ```

2. Navigate to the project directory.
   ```
   cd BashGenerator
   ```

3. Create a build directory and navigate to it.
   ```
   mkdir build
   cd build
   ```

4. Generate the Makefile using CMake.
   ```
   cmake ..
   ```

5. Build the project.
   ```
   make
   ```

## Usage

To run the program, use the following command:

```
./Main --start YYYY-MM-DD --end YYYY-MM-DD --commit X
```

Replace `YYYY-MM-DD` with the desired start and end dates, and `X` with the number of commits you want to generate.

- The generated bash files and `.txt` file with dates will be saved in a directory named `commit_START-DATE_END-DATE_X`.
- To execute the bash files, navigate to the generated directory and run:
  ```
  bash commit_0.sh "Your commit message here"
  ```

## Contributing

Feel free to submit issues and pull requests.

## License

This project is licensed under the MIT License. See the `LICENSE.md` file for details.