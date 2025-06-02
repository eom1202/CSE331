#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

// 디렉터리 생성 함수
void createDirectories() {
    // 기본 input 디렉터리 생성
    fs::create_directory("input");
    
    // 하위 디렉터리 생성
    fs::create_directory("input/sorted");
    fs::create_directory("input/random");
    fs::create_directory("input/partially_sorted");
}

// 파일 이름 생성 함수
std::string generateFileName(int size, const std::string& type, int number = 0) {
    std::string fileName = "input/";
    
    if (type == "ascending" || type == "descending") {
        fileName += "sorted/";
    } else if (type == "random") {
        fileName += "random/";
    } else if (type == "partially_sorted") {
        fileName += "partially_sorted/";
    }
    
    fileName += std::to_string(size) + "_" + type;
    
    if (number > 0) {
        fileName += "_" + std::to_string(number);
    }
    
    fileName += ".txt";
    
    return fileName;
}

// 정렬된 데이터 생성 함수
void generateSortedData(int size) {
    // 오름차순 데이터 생성
    std::string fileNameAsc = generateFileName(size, "ascending");
    std::ofstream fileAsc(fileNameAsc);
    for (int i = 1; i <= size; i++) {
        fileAsc << i << "\n";
    }
    fileAsc.close();
    std::cout << "Generated: " << fileNameAsc << std::endl;
    
    // 내림차순 데이터 생성
    std::string fileNameDesc = generateFileName(size, "descending");
    std::ofstream fileDesc(fileNameDesc);
    for (int i = size; i >= 1; i--) {
        fileDesc << i << "\n";
    }
    fileDesc.close();
    std::cout << "Generated: " << fileNameDesc << std::endl;
    
}

// 무작위 데이터 생성 함수
void generateRandomData(int size, int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, size);
    
    for (int i = 1; i <= count; i++) {
        std::string fileName = generateFileName(size, "random", i);
        std::ofstream file(fileName);
        
        // 1부터 size까지 숫자 생성
        std::vector<int> numbers(size);
        for (int j = 0; j < size; j++) {
            numbers[j] = j + 1;
        }
        
        // 무작위로 섞기
        std::shuffle(numbers.begin(), numbers.end(), gen);
        
        // 파일에 쓰기
        for (int num : numbers) {
            file << num << "\n";
        }
        
        file.close();
        std::cout << "Generated: " << fileName << std::endl;
    }
}

// 부분 정렬 데이터 생성 함수
void generatePartiallySortedData(int size, int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    for (int i = 1; i <= count; i++) {
        std::string fileName = generateFileName(size, "partially_sorted", i);
        std::ofstream file(fileName);
        
        // 1부터 size까지 숫자 생성
        std::vector<int> numbers(size);
        for (int j = 0; j < size; j++) {
            numbers[j] = j + 1;
        }
        
        // 부분 정렬을 위해 약 30%의 요소만 섞음
        int shuffleCount = size * 0.3;
        for (int j = 0; j < shuffleCount; j++) {
            int idx1 = std::uniform_int_distribution<>(0, size - 1)(gen);
            int idx2 = std::uniform_int_distribution<>(0, size - 1)(gen);
            std::swap(numbers[idx1], numbers[idx2]);
        }
        
        // 파일에 쓰기
        for (int num : numbers) {
            file << num << "\n";
        }
        
        file.close();
        std::cout << "Generated: " << fileName << std::endl;
    }
}

int main() {
    // 디렉터리 생성
    createDirectories();
    
    // 데이터 크기 배열
    std::vector<int> sizes = {150000}; // 1K, 10K, 100K, 1M
    
    for (int size : sizes) {
        // 정렬된 데이터 생성 (오름차순, 내림차순)
        generateSortedData(size);
        
        // 무작위 데이터 생성 (10개)
        generateRandomData(size, 10);
        
        // 부분 정렬 데이터 생성 (10개)
        generatePartiallySortedData(size, 10);
    }
    
    std::cout << "All input files have been generated successfully." << std::endl;
    
    return 0;
}