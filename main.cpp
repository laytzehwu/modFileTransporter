#include <iostream>
#include <thread>
#include <string>
#include <chrono>

#include <fstream>
#include <sys/stat.h>
#include <cstdio>    // fopen, fclose, fread, fwrite, BUFSIZ

using namespace std;


const std::string START_MESSAGE = "Press p and [enter] to pause.";
const std::string PAUSE_MESSAGE = "Pause! Press r and [enter] to resume.";
const std::string RESUME_MESSAGE = "Resume! Press p and [enter] to pause.";
const std::string FILE_COPIED_MESSAGE = "File is copied!";
const std::string INVALID_PARAM_MESSAGE = "Required at least 3 parameters.";
const std::string DONOT_EXISTED_MESSAGE = " does not exist.";
const std::string CANNOT_OVERWRITE_MESSAGE = " can not overwrite to itself.";
const std::string PERCENT_COPIED_MESSAGE = "% copied.";


bool bFileCopied = false;
bool bPause = false;
/**
 * Get file size
 @param - String - File path
 @return - Long - Size of file in byte
*/
long getFileSize(string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

/**
 * File copy
 @param - String - Source file path
 @param - String - Targe file path
*/
void copyFile(string sSource, string sDest) {
    std::cout << "Copying " << sSource << " to " << sDest << endl;
    std::cout << START_MESSAGE << endl;
    long lFileSize = getFileSize(sSource);

    char buf[BUFSIZ];
    size_t size;

    FILE* pSource = fopen(sSource.c_str(), "rb");
    FILE* pDest = fopen(sDest.c_str(), "wb");
    long lCopiedSize = 0;
    while(!bFileCopied) {
        if(!bPause) {
            if(size = fread(buf, 1, BUFSIZ, pSource)) {
                fwrite(buf, 1, size, pDest);
                lCopiedSize += (long) size;
                float fPercent = (float) lCopiedSize /lFileSize * 100;
                std::cout << fPercent << PERCENT_COPIED_MESSAGE << endl;
            } else {
                bFileCopied = false;
                break;
            }
        }
        // Sleep for 1 second to release resource.
        std::this_thread::sleep_for (std::chrono::seconds(1));

    }


    fclose(pSource);
    fclose(pDest);

    bFileCopied = true;
    std::cout << endl;
    std::cout << FILE_COPIED_MESSAGE << endl;
}

/**
 * Pause file copying
*/
void doPause() {

    if(bPause) return;
    std::cout << endl;
    std::cout << PAUSE_MESSAGE << endl;
    std::cout.flush();
    bPause = true;
}

/**
 * Resume file copying
*/
void doResume() {
    if(!bPause) return;
    std::cout << endl;
    std::cout << RESUME_MESSAGE << endl;
    std::cout.flush();
    bPause = false;
}

/**
 *Monotor keypress and trigger pause and resume
*/
void monitorKeyPress() {
    char cKey = 'N';
    while(!bFileCopied) {
        std::cin >> cKey;
        switch(cKey) {
            case 'p':
            case 'P':
                doPause();
                break;
            case 'r':
            case 'R':
                doResume();
                break;
        }
    }

}

/**
 * Check whether the file is existed
 @param - String - File path
 @return - Boolean - Flag tells the file is existed
*/
bool isFileExisted(string sFileName)
{
    const char* pFileName = sFileName.c_str();
    std::ifstream infile(pFileName);
    return infile.good();
}

/**
 * Array target file path
 * Append source file name while it is not set
 * @param - String - Targe file path
 * @param - String - Source file path
*/
void arrayPath(string &sDestPath, string sSourcePath) {
    // Skip while it came with a file name
    if(sDestPath.back() != '/' && sDestPath.back() != '\\') return;
    size_t iLastSlashIdx = sSourcePath.find_last_of('/');
    if(string::npos == iLastSlashIdx) {
        iLastSlashIdx = sSourcePath.find_last_of('\\');
    }

    string fileName = "";
    if(string::npos == iLastSlashIdx) {
        fileName = sSourcePath;
    } else {
        fileName = sSourcePath.substr(iLastSlashIdx + 1);
    }
    sDestPath += fileName;
}

/**
 * Targe file overwriting confirmation
 @param - String - File path
 @return - Boolean - User agree?
*/
bool confirmOverWriteFile(string sPath) {
    bool bResult = false;
    char cAnswer;
    bool bConfirm = false;
    while(true) {
         std::cout << "Overwrite " << sPath << "?" << endl;
         std::cout << "Y-yes; N-No => ";
         std::cin >> cAnswer;
         switch(cAnswer) {
             case 'Y':
             case 'y':
               bResult = true;
             case 'n':
             case 'N':
                bConfirm = true;
                break;
         }
         if(bConfirm) break;
    }

    return bResult;
}

int main(int argc, char* argv[])
{
    if(argc < 3) {
        std::cout << INVALID_PARAM_MESSAGE << endl;
        return 0;
    }

    string sSourcePath = argv[1];

    if(!isFileExisted(sSourcePath)) {
        std::cout << sSourcePath << DONOT_EXISTED_MESSAGE << endl;
        return 0;
    }

    string sDestPath = argv[2];
    arrayPath(sDestPath, sSourcePath); // Arrange target file to actual file name
    if(sSourcePath == sDestPath) {
        std::cout << sSourcePath << CANNOT_OVERWRITE_MESSAGE << endl;
        return 0;
    }

    if(isFileExisted(sDestPath)) {
        if(!confirmOverWriteFile(sDestPath)) return 0;
    }
    std::thread fileCopyThread (copyFile, sSourcePath, sDestPath);
    std::thread keyMonitorThread (monitorKeyPress);
    keyMonitorThread.detach();
    fileCopyThread.join();

    return 0;
}
