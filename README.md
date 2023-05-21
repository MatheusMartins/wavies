**`wavies`** is a copy utility with classification and deduplication features


The default rules are:

- WAV files 
    - If SampleRate >= 44.1k + BitDepth >= 16-bit + NumChannels >= 2 
    Is copied into: </output/path/>hd/ 

- Any new file 
    Is copied into: </output/path/>default/ 

- Any duplicated file 
    Is renamed and copied into </output/path/>archive/
    
        
!!! This software doesn't delete any files by default,
!!! but always be careful with your files

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