#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <algorithm>

// 기존 코드에서 가져온 함수들은 주석으로 표시하지 않음

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

// 부분 배열에 대한 삽입 정렬
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

// 부분 배열에 대한 힙 정렬
void partialHeapSort(std::vector<int>& arr, int left, int right) {
    int n = right - left + 1;
    
    // 부분 배열을 임시 벡터로 복사
    std::vector<int> temp(arr.begin() + left, arr.begin() + right + 1);
    
    // 힙 정렬 수행
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(temp, n, i);
    
    for (int i = n - 1; i > 0; i--) {
        std::swap(temp[0], temp[i]);
        heapify(temp, i, 0);
    }
    
    // 정렬된 결과를 원본 배열에 복사
    for (int i = 0; i < n; i++) {
        arr[left + i] = temp[i];
    }
}

int partition(std::vector<int>& arr, int low, int high) {
    // 피벗 선택 개선 (중간값 사용)
    int mid = low + (high - low) / 2;
    
    // 중간값을 피벗으로 선택하기 위한 정렬
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

// 인트로 정렬의 메인 알고리즘
void introSortUtil(std::vector<int>& arr, int low, int high, int depthLimit) {
    // 부분 배열의 크기
    int size = high - low + 1;
    
    // 작은 배열에는 삽입 정렬 사용
    if (size < 16) {
        partialInsertionSort(arr, low, high);
        return;
    }
    
    // 재귀 깊이 제한에 도달했으면 힙 정렬로 전환
    if (depthLimit == 0) {
        partialHeapSort(arr, low, high);
        return;
    }
    
    // 퀵 정렬의 분할 사용
    int pivotPos = partition(arr, low, high);
    
    // 분할 후 재귀적으로 정렬
    if (pivotPos > low) {
        introSortUtil(arr, low, pivotPos - 1, depthLimit - 1);
    }
    if (pivotPos < high) {
        introSortUtil(arr, pivotPos + 1, high, depthLimit - 1);
    }
}

// 인트로 정렬 메인 함수
void introSort(std::vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;
    
    // 최대 재귀 깊이 계산 (2 * log2(n))
    int maxDepth = 2 * std::log2(n);
    
    // 인트로 정렬 시작
    introSortUtil(arr, 0, n - 1, maxDepth);
}

// 배열 출력 함수
void printArray(const std::vector<int>& arr) {
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

int main() {
    // 테스트 케이스 1: 무작위 배열
    std::vector<int> arr1 = {64, 34, 25, 12, 22, 11, 90};
    std::cout << "원본 배열 1: ";
    printArray(arr1);
    introSort(arr1);
    std::cout << "정렬 후 배열 1: ";
    printArray(arr1);
    
    // 테스트 케이스 2: 역순으로 정렬된 배열
    std::vector<int> arr2 = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::cout << "\n원본 배열 2: ";
    printArray(arr2);
    introSort(arr2);
    std::cout << "정렬 후 배열 2: ";
    printArray(arr2);
    
    // 테스트 케이스 3: 이미 정렬된 배열
    std::vector<int> arr3 = {1, 2, 3, 4, 5};
    std::cout << "\n원본 배열 3: ";
    printArray(arr3);
    introSort(arr3);
    std::cout << "정렬 후 배열 3: ";
    printArray(arr3);
    
    // 테스트 케이스 4: 중복 값이 있는 배열
    std::vector<int> arr4 = {3, 3, 1, 5, 5, 2};
    std::cout << "\n원본 배열 4: ";
    printArray(arr4);
    introSort(arr4);
    std::cout << "정렬 후 배열 4: ";
    printArray(arr4);
    
    // 테스트 케이스 5: 더 큰 무작위 배열
    std::vector<int> arr5;
    for (int i = 0; i < 30; i++) {
        arr5.push_back(rand() % 100);
    }
    std::cout << "\n원본 배열 5: ";
    printArray(arr5);
    introSort(arr5);
    std::cout << "정렬 후 배열 5: ";
    printArray(arr5);
    
    return 0;
}