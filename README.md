wavies
-------

**`wavies`** is a copy utility with classification and deduplication features


The default rules are:

- WAV files 
    - If SampleRate >= 44.1k + BitDepth >= 16-bit + NumChannels >= 2 
    
        -> Is copied into: </output/path/>hd/ 

- Any new file 
    
    -> Is copied into: </output/path/>default/ 

- Any duplicated file 
    
    -> Is renamed and copied into </output/path/>archive/
    
        
!! This software doesn't delete any files by default, but always be careful with your files !!

**Installation:**
```
  mkdir ./wavies
  cd ./wavies
  git clone https://github.com/MatheusMartins/wavies.git .
  g++ wavies.cpp -o wavies
```
The above commands will:
- create a `wavies` folder
- change the working directory into the new folder
- clone this program using git
- build the **`wavies`** executable using g++

**Usage:**
```
  wavies [options] </input/path/> </output/path/>
```

**Options:**
```
        -f              Flatten output files within the same parent folder
                        excluding any intermediary folder.
        -h              Show this help message
        -l              Creates logs: directories.log, files.log
        -v              Enables verbose mode
```

**Example:**
```
  wavies -f /home/user/Music/ /home/user/wavies/Music/
```


Dependencies
-------
This software was only possible with the useof the below other software and is accordingly licensed.

- https://github.com/adamstark/AudioFile


License
-------

MIT License

Copyright (c) 2017 Adam Stark

Copyright (c) 2023 Matheus Martins

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
