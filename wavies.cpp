#include <iostream>
#include <filesystem>
#include <fstream>
#include "lib/AudioFile.h"

using namespace std;
namespace fs = filesystem;

void help(){
    string text = 
        "wavies is a copy utility with classification and deduplication features\n"
        "  The default rules are:\n"
        "    - WAV files \n"
        "      - If SampleRate >= 44.1k + BitDepth >= 16-bit + NumChannels >= 2 \n"
        "        Is copied into: </output/path/>hd/ \n"
        "    - Any new file \n"
        "        Is copied into: </output/path/>default/ \n"
        "    - Any duplicated file \n"
        "        Is copied into </output/path/>archive/ \n"
        "!! This software doesn't delete any files by default,\n"
        "!! but always be careful with your files \n"
        "\nUsage: \n"
        "  wavies [options] </input/path/> </output/path/>\n"
        "\nOptions: \n"
        "  \t-f\t\tFlatten output files within the same parent folder\n"
        "  \t  \t\texcluding any intermediary folder.\n"
        "  \t-h\t\tShow this help message\n"
        "  \t-l\t\tCreates logs: directories.log, files.log\n"
        "  \t-v\t\tEnables verbose mode\n"
        "\nExample: \n"
        "  wavies -f /home/user/Music/ /home/user/wavies/Music/\n";
    cout << text << endl;
}

struct Settings 
{
    bool flatten = false;
    bool logs = false;
    bool verbose = false;
};

struct Dummyfile
{
    std::string name; 
    std::string originalUrl;
    std::string newUrl; 
    std::string extension;
    std::string prefix;
    
    bool isDuplicate(vector<Dummyfile> filelist) {
        for(Dummyfile f : filelist){
            if(f.name == name) {
                return true;
            }
        }
        return false;
    }
};


struct Dummyfolder
{
    std::string name;
    std::string originalUrl; 
    std::string prefixUrl;
    std::string trimmedUrl;
};


std::vector<std::string> split(std::string s, std::string delimiter) {
    /**
     * Splits strings based on delimiters
    */
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}


int main(int argc, char* argv[]) {
    /**
     * Parse options and arguments
    */
    Settings settings;
    string outputPath, inputPath, options;

    if (argc <= 2 || argc > 4) {
        help();
        return -1;
    } else if (argc == 3) {
        inputPath  = argv[1];
        outputPath = argv[2];
    } else if (argc == 4) {
        options    = argv[1];
        inputPath  = argv[2];
        outputPath = argv[3];
    } else {
        return -1;
    }
    for (char o : options) {
        if(o == 'h') {
            help();
            return -1;
        } 
        if(o == 'f') {
            settings.flatten = true;
            cout << "option -f <flatten> selected: will flatten all outputs in same parent folder." << endl;
        } 
        if(o == 'v') {
            settings.verbose = true;
        } 
        if(o == 'l') {
            settings.logs = true;
        } 
    }

    /**
     * Validates directories exist
    */
    if(!fs::is_directory(outputPath)){
        cout << "Output directory doesn't exist: \n\t" << outputPath << endl;
        cout << "Do you want it to be created now? (Y / N) ";
        int create_out_dir = cin.get();
        if(create_out_dir == 121 || create_out_dir == 89) {
            fs::create_directory(outputPath);
        } else {
            cout << create_out_dir;
            cout << "Please create the output directory: \n\t" << outputPath << endl;
            return -1;
        }
    }
    if(!fs::is_directory(inputPath)){
        cout << "Please create the input directory: \n\t" << inputPath << endl;
        return -1;
    }

    vector<Dummyfolder> directories;
    vector<Dummyfile> files;

    /**
     * Recursive loop on inputPath
    */
    for(const auto& dirEntry : fs::recursive_directory_iterator(fs::path(inputPath))){
        if(dirEntry.is_directory()) {
            /**
             * Directories
            */
            Dummyfolder currFolder;
            currFolder.originalUrl = dirEntry.path();
            currFolder.prefixUrl = "";
            currFolder.trimmedUrl = split(currFolder.originalUrl, inputPath)[1];

            std::vector<std::string> v = split(currFolder.originalUrl, "/");
            for (auto i : v){
                if (v.at(v.size()-1) == i) {
                    currFolder.name = i;
                } else {
                    currFolder.prefixUrl += "/" + i;
                }
            } 
            directories.push_back(currFolder);

        } else if(dirEntry.is_regular_file()) {
            /**
             * Files
            */
            Dummyfile currFile;
            currFile.originalUrl = dirEntry.path();
            currFile.extension = currFile.originalUrl.substr(currFile.originalUrl.size()-3);
            currFile.newUrl = outputPath;

            std::vector<std::string> v = split(currFile.originalUrl, "/");
            for (auto i : v){
                if (v.at(v.size()-1) == i) {
                    currFile.name = i;
                }
            } 

            string split1 = split(currFile.originalUrl, inputPath)[1];
            currFile.prefix = split(split1, currFile.name)[0];

            if(currFile.isDuplicate(files)){
                /**
                 * Duplicates
                */
                stringstream trashName;
                trashName << "dup_" << files.size() << "_" << currFile.name;
                string trashFolder = outputPath + "/archive/";
                if(!fs::is_directory(trashFolder)){
                    fs::create_directory(trashFolder);
                }
                string trashUrl = trashFolder + trashName.str();
                if(!fs::is_regular_file(trashUrl)) {
                    fs::copy_file(currFile.originalUrl, trashUrl);
                }
                files.push_back(currFile);
            } else {
                if(currFile.extension == "wav") {
                    /**
                     * Checking for quality in WAV files
                     *      powered by AudioFile (https://github.com/adamstark/AudioFile)
                     *      
                    */
                    AudioFile<double> audioFile;
                    audioFile.load(currFile.originalUrl);
                    int sampleRate = audioFile.getSampleRate();
                    int bitDepth = audioFile.getBitDepth(); 
                    int numChannels = audioFile.getNumChannels();
                    if(settings.verbose) {
                        audioFile.printSummary();
                    }
                    if(sampleRate >= 44100 && bitDepth >= 16 && numChannels >= 2) {
                        // Categorize as 'hd' for good quality
                        currFile.newUrl += "/hd/";
                    } else {
                        // Categorize as 'listenable' 
                        currFile.newUrl += "/default/";
                    }
                } else {
                    // Not acceptable
                    currFile.newUrl += "/default/";
                }
                
                // Flatten flag is NOT set, append original file intermediary folders
                if(!settings.flatten){
                    currFile.newUrl += currFile.prefix;
                }

                // Path processing is done, append final name.
                currFile.newUrl += currFile.name;

                if(currFile.newUrl.length() > 0){
                    string dir_name = split(currFile.newUrl, currFile.name)[0];
                    if(!fs::is_directory(dir_name)) {
                        string check_folder;
                        for (string folder : split(dir_name, "/")) {
                            check_folder += "/" + folder;
                            if(check_folder.find(outputPath) != std::string::npos){
                                if(!fs::is_directory(check_folder)){
                                    fs::create_directory(check_folder);
                                }
                            }
                        }
                    }
                    if(!fs::is_regular_file(currFile.newUrl)){
                        fs::copy_file(currFile.originalUrl, currFile.newUrl);
                    }
                    files.push_back(currFile);
                }
            }
        } else {
            cout << "There's an unknown item in the directory and it will be ignored!" << endl;
        }
    }

    /**
     * Simple log of processed directories and files
    */
    if (settings.logs) {
        vector<string> dir_names;
        for(Dummyfolder f : directories) {
            dir_names.push_back(f.name);
        }
        vector<string> file_names;
        for(Dummyfile f : files) {
            file_names.push_back(f.name);
        }
        ofstream log_dir("./directories.log");
        ostream_iterator<std::string> log_dir_iterator(log_dir, "\n");
        std::copy(std::begin(dir_names), std::end(dir_names), log_dir_iterator);
        
        ofstream log_files("./files.log");
        ostream_iterator<std::string> log_files_iterator(log_files, "\n");
        std::copy(std::begin(file_names), std::end(file_names), log_files_iterator);
    }
}
