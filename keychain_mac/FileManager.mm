//
//  FileManager.m
//  keychain
//
//  Created by Mikhail Lutskiy on 03/12/2018.
//

#import "FileManager.h"
#include <vector>
#include <boost/filesystem.hpp>
#include <mach-o/dyld.h>

@implementation FileManager

+ (NSString *) getWorkDirectoryPath {
    auto a = get_work_path();
    return [NSString stringWithUTF8String:a.parent_path().c_str()];
}

+ (NSString *) getBinaryPath {
    auto a = get_work_path();
    return [NSString stringWithUTF8String:a.c_str()];
}

boost::filesystem::path get_work_path() {
    unsigned int bufferSize = 512;
    std::vector<char> buffer(bufferSize + 1);
    if(_NSGetExecutablePath(&buffer[0], &bufferSize))
    {
        buffer.resize(bufferSize);
        _NSGetExecutablePath(&buffer[0], &bufferSize);
    }
    boost::filesystem::path path = &buffer[0];
    return path;
}


@end
