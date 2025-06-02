#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>
#include <filesystem>
#include <iomanip>
#include <cmath>
#include <climits>

namespace fs = std::filesystem;

// --------------- 정렬 알고리즘 구현  ---------------

void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void selectionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        std::swap(arr[i], arr[min_idx]);
    }
}

void insertionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    std::vector<int> L(n1), R(n2);
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSortImpl(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        mergeSortImpl(arr, left, mid);
        mergeSortImpl(arr, mid + 1, right);
        
        merge(arr, left, mid, right);
    }
}

void mergeSort(std::vector<int>& arr) {
    mergeSortImpl(arr, 0, arr.size() - 1);
}

int partition(std::vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    
    if (arr[mid] < arr[low])
        std::swap(arr[low], arr[mid]);
    if (arr[high] < arr[low])
        std::swap(arr[low], arr[high]);
    if (arr[mid] < arr[high])
        std::swap(arr[mid], arr[high]);
    
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortImpl(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        
        quickSortImpl(arr, low, pi - 1);
        quickSortImpl(arr, pi + 1, high);
    }
}

void quickSort(std::vector<int>& arr) {
    quickSortImpl(arr, 0, arr.size() - 1);
}

void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && arr[left] > arr[largest])
        largest = left;
    
    if (right < n && arr[right] > arr[largest])
        largest = right;
    
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(std::vector<int>& arr) {
    int n = arr.size();
    
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void librarySort(std::vector<int>& arr) {
    int n = arr.size();
    int lib_size, index_pos,
        *gaps,
        *library[2];
 
    bool target_lib, *numbered;
 
    for (int i = 0; i < 2; i++)
        library[i] = new int[n];
 
    gaps = new int[n + 1];
    numbered = new bool[n + 1];
    
    for (int i = 0; i <= n; i++) {
        numbered[i] = false;
    }
 
    lib_size = 1;
    index_pos = 1;
    target_lib = 0;
    library[target_lib][0] = arr[0];
 
    while (index_pos < n) {
        int insert = std::distance(
            library[target_lib],
            std::lower_bound(library[target_lib],
                             library[target_lib] + lib_size, arr[index_pos]));
 
        if (numbered[insert] == true) {
            int prov_size = 0, next_target_lib = !target_lib;
  
            for (int i = 0; i <= n; i++) {
                if (numbered[i] == true) {
                    library[next_target_lib][prov_size] = gaps[i];
                    prov_size++;
                    numbered[i] = false;
                }
 
                if (i <= lib_size) {
                    library[next_target_lib][prov_size] =
                        library[target_lib][i];
                    prov_size++;
                }
            }
 
            target_lib = next_target_lib;
            lib_size = prov_size - 1;
        } else {
            numbered[insert] = true;
            gaps[insert] = arr[index_pos];
            index_pos++;
        }
    }
 
    int index_pos_for_output = 0;
    for (int i = 0; index_pos_for_output < n; i++) {
        if (numbered[i] == true) {
            arr[index_pos_for_output] = gaps[i];
            index_pos_for_output++;
        }
 
        if (i < lib_size) {
            arr[index_pos_for_output] = library[target_lib][i];
            index_pos_for_output++;
        }
    }
    
    for (int i = 0; i < 2; i++) {
        delete[] library[i];
    }
    delete[] gaps;
    delete[] numbered;
}

void partialInsertionSort(std::vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void timSort(std::vector<int>& arr) {
    int n = arr.size();
    int run = 32;

    for (int i = 0; i < n; i += run) {
        int end = std::min(i + run - 1, n - 1);
        partialInsertionSort(arr, i, end);
    }
    
    int size = run;
    while (size < n) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = std::min(left + 2 * size - 1, n - 1);
            
            if (mid < right) {
                merge(arr, left, mid, right);
            }
        }
        size *= 2;
    }
}

void cocktailSort(std::vector<int>& arr) {
    int begin = 0;
    int end = arr.size() - 1;
    int shift = begin;
    int i;

    while (begin < end) {
        i = begin;
        while (i < end) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                shift = i;
            }
            i++;
        }
        end = shift;

        i = end;
        while (i > begin) {
            if (arr[i] < arr[i - 1]) {
                std::swap(arr[i], arr[i - 1]);
                shift = i;
            }
            i--;
        }
        begin = shift;
    }
}

void combSort(std::vector<int>& arr) {
    int n = arr.size();
    int gap = n;
    float shrink = 1.3;
    bool sorted = false;
    
    while (!sorted) {
        gap = floor(gap / shrink);
        
        if (gap > 1) {
            sorted = false;
        } else {
            gap = 1;
            sorted = true;
        }
        
        int i = 0;
        while (i + gap < n) {
            if (arr[i] > arr[i + gap]) {
                std::swap(arr[i], arr[i + gap]);
                sorted = false;
            }
            i++;
        }
    }
}

void tournamentSort(std::vector<int>& arr) {
    int n = arr.size();
    std::vector<int> tmp(2 * n);
    const int INF = INT_MAX;
    
    auto winner = [&](int pos1, int pos2) -> int {
        int u = pos1 >= n ? pos1 : tmp[pos1];
        int v = pos2 >= n ? pos2 : tmp[pos2];
        
        if (tmp[u] <= tmp[v]) return u;
        return v;
    };
    
    auto createTree = [&](int& value) {
        for (int i = 0; i < n; i++) {
            tmp[n + i] = arr[i];
        }
        
        for (int i = 2 * n - 1; i > 1; i -= 2) {
            int k = i / 2;
            int j = i - 1;
            tmp[k] = winner(i, j);
        }
        
        value = tmp[tmp[1]];
        tmp[tmp[1]] = INF;
    };
    
    auto recreate = [&](int& value) {
        int i = tmp[1];
        
        while (i > 1) {
            int j, k = i / 2;
            
            if (i % 2 == 0 && i < 2 * n - 1) {
                j = i + 1;
            } else {
                j = i - 1;
            }
            
            tmp[k] = winner(i, j);
            i = k;
        }
        
        value = tmp[tmp[1]];
        tmp[tmp[1]] = INF;
    };
    
    int value;
    createTree(value);
    
    for (int i = 0; i < n; i++) {
        arr[i] = value;
        recreate(value);
    }
}

void partialHeapSort(std::vector<int>& arr, int left, int right) {
    int n = right - left + 1;
    
    std::vector<int> temp(arr.begin() + left, arr.begin() + right + 1);
    
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(temp, n, i);
    
    for (int i = n - 1; i > 0; i--) {
        std::swap(temp[0], temp[i]);
        heapify(temp, i, 0);
    }
    
    for (int i = 0; i < n; i++) {
        arr[left + i] = temp[i];
    }
}

void introSortUtil(std::vector<int>& arr, int low, int high, int depthLimit) {
    int size = high - low + 1;
    
    if (size < 16) {
        partialInsertionSort(arr, low, high);
        return;
    }
    
    if (depthLimit == 0) {
        partialHeapSort(arr, low, high);
        return;
    }
    
    int pivotPos = partition(arr, low, high);
    
    if (pivotPos > low) {
        introSortUtil(arr, low, pivotPos - 1, depthLimit - 1);
    }
    if (pivotPos < high) {
        introSortUtil(arr, pivotPos + 1, high, depthLimit - 1);
    }
}

void introSort(std::vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;
    
    int maxDepth = 2 * std::log2(n);
    
    introSortUtil(arr, 0, n - 1, maxDepth);
}


// --------------- 테스트 관련 함수 ---------------

// 정렬이 올바르게 되었는지 확인하는 함수
bool isSorted(const std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i] < arr[i - 1]) {
            return false;
        }
    }
    return true;
}

// 파일에서 데이터 읽기
std::vector<int> readDataFromFile(const std::string& fileName) {
    std::vector<int> data;
    std::ifstream file(fileName);
    
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return data;
    }
    
    int value;
    while (file >> value) {
        data.push_back(value);
    }
    
    file.close();
    return data;
}

// 각 알고리즘을 테스트하는 함수
void testSortingAlgorithm(const std::string& algorithmName,
    void (*sortFunction)(std::vector<int>&),
    const std::string& inputDirectory,
    const std::string& outputDirectory) {

// 결과를 저장할 CSV 파일 생성
std::string resultFileName = outputDirectory + "/" + algorithmName + "/results.csv";

// 알고리즘 결과 디렉터리 생성
fs::create_directories(outputDirectory + "/" + algorithmName);

std::ofstream resultFile(resultFileName);
resultFile << "Size,Type,FileNumber,RunningTime(ms),Accuracy\n";

// 데이터 크기 배열
std::vector<int> sizes = {1000, 10000, 100000, 150000}; // 1K, 10K, 100K, 1M

// 정렬 기준 배열
std::vector<std::string> sortTypes = {"sorted/ascending", "sorted/descending",
                   "random", "partially_sorted"};

for (const auto& sortType : sortTypes) {
for (int size : sizes) {

if ((algorithmName == "BubbleSort" || algorithmName == "SelectionSort" || algorithmName == "InsertionSort" ||
    algorithmName == "CocktailSort") && size >= 1000000) {
std::cout << "\nSkipping test for: " << algorithmName << " - " << sortType << " - Size: " << size << " (O(n^2) algorithm, size >= 1M)" << std::endl;
continue;
}  

if ((algorithmName == "QuickSort" && 
    (sortType == "sorted/ascending" || sortType == "sorted/descending") && 
    size >= 1000000) ||
    (algorithmName == "LibrarySort" && 
    (sortType == "sorted/ascending" || sortType == "sorted/descending" || sortType == "partially_sorted") && 
    size >= 1000000)) {
    std::cout << "\nSkipping test for: " << algorithmName << " - " << sortType << " - 크기: " << size 
              << " (정렬된 데이터에서 크기가 1M 이상일 때 스택 오버플로우 발생 가능)" << std::endl;
    continue;
}

// *** 수정된 부분 끝 ***

std::cout << "\n테스트 중: " << algorithmName << " - " << sortType << " - 크기: " << size << std::endl;

// 파일 패턴 결정
std::string filePattern = inputDirectory + "/" + sortType + "/" + std::to_string(size) + "_";
std::string baseName; // sortType에서 기본 이름 추출 (예: "ascending", "random")
size_t last_slash_idx = sortType.find_last_of("/");
if (std::string::npos != last_slash_idx) {
baseName = sortType.substr(last_slash_idx + 1);
} else {
baseName = sortType; // 슬래시가 없는 경우 그대로 사용
}


// 파일 개수 결정
int fileCount = 1;
if (sortType == "random" || sortType == "partially_sorted") {
fileCount = 10;
}

for (int fileNum = 1; fileNum <= fileCount; fileNum++) {
std::string fileName;

if (fileCount == 1) {
// 단일 파일 경우 (ascending, descending)
fileName = filePattern + baseName + ".txt";
} else {
// 여러 파일 경우 (random, partially_sorted)
fileName = filePattern + baseName + "_" + std::to_string(fileNum) + ".txt";
}


// 파일이 존재하는지 확인
if (!fs::exists(fileName)) {
std::cerr << "File does not exist: " << fileName << std::endl;
continue;
}

// 데이터 읽기
std::vector<int> data = readDataFromFile(fileName);

if (data.empty()) {
std::cerr << "Empty data from file: " << fileName << std::endl;
continue;
}

// 정렬 시간 측정 시작
auto start = std::chrono::high_resolution_clock::now();

// 정렬 알고리즘 실행
sortFunction(data);

// 정렬 시간 측정 종료
auto end = std::chrono::high_resolution_clock::now();

// 정렬 시간 계산 (밀리초)
double duration = std::chrono::duration<double, std::milli>(end - start).count();

// 정렬 정확도 확인
bool sorted = isSorted(data);

// 결과 출력
std::cout << "파일: " << fileName << std::endl;
std::cout << "실행 시간: " << duration << " ms" << std::endl;
std::cout << "정확도: " << (sorted ? "100%" : "오류") << std::endl;

// CSV에 결과 기록
resultFile << size << ",";
resultFile << baseName << ","; // 파일 유형 기본 이름 사용
resultFile << (fileCount == 1 ? 0 : fileNum) << ","; // 단일 파일은 0, 다중 파일은 번호
resultFile << std::fixed << std::setprecision(4) << duration << ",";
resultFile << (sorted ? "100%" : "0%") << "\n";
}
}
}

resultFile.close();
std::cout << "\n" << algorithmName << " 테스트 완료. 결과 저장됨: " << resultFileName << std::endl;
}

// --------------- main 함수 ---------------
int main() {
// 입력 및 출력 디렉터리 설정
std::string inputDirectory = "input";
std::string outputDirectory = "output";

// 출력 디렉터리 생성
fs::create_directory(outputDirectory);

// 정렬 알고리즘 및 이름을 저장할 배열
std::vector<std::pair<std::string, void (*)(std::vector<int>&)>> algorithms = {
// {"알고리즘 명", 구현된 함수} 양식으로 작성
{"BubbleSort", bubbleSort},
{"SelectionSort", selectionSort},
{"InsertionSort", insertionSort},
{"MergeSort", mergeSort},
{"QuickSort", quickSort},
{"HeapSort", heapSort},
{"LibrarySort", librarySort},
{"TimSort", timSort},
{"CocktailSort", cocktailSort},
{"CombSort", combSort},
{"TournamentSort", tournamentSort},
{"IntroSort", introSort}
};

// 각 알고리즘 테스트
for (const auto& algorithm : algorithms) {
std::cout << "\n===== " << algorithm.first << " 테스트 시작 =====" << std::endl;
// 수정된 testSortingAlgorithm 함수 호출
testSortingAlgorithm(algorithm.first, algorithm.second, inputDirectory, outputDirectory);
}

std::cout << "\n모든 테스트가 완료되었습니다." << std::endl;

return 0;
}