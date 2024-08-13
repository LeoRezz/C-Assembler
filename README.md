# Assembler Project
<pre>                                                                                    
      .o.                                                      .o8       oooo                     
     .888.                                                    "888       `888                     
    .8"888.      .oooo.o  .oooo.o  .ooooo.  ooo. .oo.  .oo.    888oooo.   888   .ooooo.  oooo d8b 
   .8' `888.    d88(  "8 d88(  "8 d88' `88b `888P"Y88bP"Y88b   d88' `88b  888  d88' `88b `888""8P 
  .88ooo8888.   `"Y88b.  `"Y88b.  888ooo888  888   888   888   888   888  888  888ooo888  888     
 .8'     `888.  o.  )88b o.  )88b 888    .o  888   888   888   888   888  888  888    .o  888     
o88o     o8888o 8""888P' 8""888P' `Y8bod8P' o888o o888o o888o  `Y8bod8P' o888o `Y8bod8P' d888b    
</pre>                                                                     



## Diagram
<pre> 
Start
│
├─ Read input file
│
├─ For each line in input file
│   │
│   ├─ Tokenize line (tokenize_line function)
│   │   │
│   │   ├─ Initialize token array
│   │   │
│   │   ├─ For each word in line
│   │   │   │
│   │   │   ├─ Extract word (my_getword function)
│   │   │   │
│   │   │   └─ Determine token type (get_token_type function)
│   │   │
│   │   └─ Return array of tokens
│   │
│   ├─ Parse line (parse_line function)
│   │   │
│   │   ├─ Check for label
│   │   │
│   │   ├─ Identify line type (instruction/data/directive)
│   │   │
│   │   ├─ If instruction
│   │   │   │
│   │   │   ├─ Identify opcode
│   │   │   │
│   │   │   └─ Parse operands
│   │   │
│   │   ├─ If data
│   │   │   │
│   │   │   └─ Parse data values
│   │   │
│   │   └─ If directive
│   │       │
│   │       └─ Handle directive
│   │
│   ├─ Update symbol table if necessary
│   │
│   └─ Update IC or DC
│
├─ End of first pass
│
├─ Update data symbols
│
├─ Print symbol table
│
└─ End
</pre> 

## Description
[Provide a brief description of your assembler project. Explain its purpose and main features.]

## Table of Contents
1. [Project Overview](#project-overview)
2. [Installation](#installation)
3. [Usage](#usage)
4. [Project Structure](#project-structure)
5. [Features](#features)
6. [Known Issues](#known-issues)
7. [Contributors](#contributors)

## Project Overview
This assembler is a student project developed as part of [Course Name] at [University/College Name]. It aims to [brief explanation of the project's goals].

## Installation
[Explain how to install and set up your project. Include any dependencies that need to be installed and steps to compile the project.]

## Usage
[Provide instructions on how to use your assembler. Include example commands and explain the input/output format.]

## Project Structure
[List and briefly describe the main components of your project, such as:]
- `tokenizer.c/.h`: [Description]
- `parser.c/.h`: [Description]
- `symbolTable.c/.h`: [Description]
- `firstPass.c`: [Description]
- [Add other important files]

## Features
[List the key features of your assembler, such as:]
- Two-pass assembly process
- Support for [list supported instructions and directives]
- Error reporting
- [Add other features]

## Known Issues
[List any known bugs or limitations in the current version.]

## Contributors
This project was developed by:
- Lior Mor
- Vikki

As part of Systems Programming Laboratory under the guidance of [Instructor's Name].
