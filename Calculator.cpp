#include "Calculator.h"

Calculator::Calculator() {

}

Calculator::Calculator(int rows, int columns) {
  this->rows = rows;
  this->columns = columns;
  creatorArray = new ArrayCreator(rows, columns);
  newArray = creatorArray->GetArray();
  secondGen = creatorArray->GetNumbers();
  firstGen = creatorArray->GetNumbers();
  currentGen = creatorArray->GetNumbers();
  totalNeighborCount = creatorArray->GetDouble();
  generationNumber = 0;
}

Calculator::Calculator(int rows, int columns, char** myArray) {
  this->rows = rows;
  this->columns = columns;
  creatorArray = new ArrayCreator(rows, columns);
  secondGen = creatorArray->GetNumbers();
  firstGen = creatorArray->GetNumbers();
  currentGen = creatorArray->GetNumbers();
  totalNeighborCount = creatorArray->GetDouble();
  newArray = myArray;
  generationNumber = 0;
}

Calculator::~Calculator() {
  for (int i = 0; i < rows; i++){
      delete[] newArray[i];
      delete[] secondGen[i];
      delete[] firstGen[i];
      delete[] currentGen[i];
      delete[] totalNeighborCount[i];
    }

  delete[] newArray;
  delete[] secondGen;
  delete[] firstGen;
  delete[] currentGen;
  delete[] totalNeighborCount;
  delete creatorArray;
}

void Calculator::DetermineStartingLocations(double popDense) {
  int count = 0;
  int totalCount = rows * columns;
  int numCells = totalCount * popDense;

  while (count < numCells) {
    int r = GetNumbers(rows);
    int c = GetNumbers(columns);
    if (newArray[r][c] == 'X') {
      r = (++r) % rows;
      c = (++c) % columns;

      if (newArray[r][c] == 'X') {
        continue;
      }
    }
    newArray[r][c] = 'X';
    count++;
  }
}

void Calculator::ClassicModeCalculator() {
  CentralChunk();
  EdgeCalculator();
  IncrementTotalNeighbors();
  CreateNewGeneration();
  MoveGenerationsDown();
  generationNumber++;
}

void Calculator::IncrementTotalNeighbors() {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      if (generationNumber == 0) {
        totalNeighborCount[i][j] = currentGen[i][j];
      }
      else if (generationNumber == 1) {
        totalNeighborCount[i][j] = double((currentGen[i][j] + firstGen[i][j]) / 2.0);
      }
      else {
        totalNeighborCount[i][j] = double((currentGen[i][j] + firstGen[i][j] + secondGen[i][j]) / 3.0);
      }
    }
  }
}

void Calculator::MoveGenerationsDown() {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      if (generationNumber == 0) {
        firstGen[i][j] = currentGen[i][j];
      }
      else {
        secondGen[i][j] = firstGen[i][j];
        firstGen[i][j] = currentGen[i][j];
      }
    }
  }
}

void Calculator::CreateNewGeneration() {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      if (totalNeighborCount[i][j] < 1.5) {
        newArray[i][j] = '-';
      }
      else if (totalNeighborCount[i][j] <= 2.5) {
        newArray[i][j] = newArray[i][j];
      }
      else if (totalNeighborCount[i][j] <= 3.5) {
        newArray[i][j] = 'X';
      }
      else {
        newArray[i][j] = '-';
      }
    }
  }
}

bool Calculator::isDone() {
  int stable = 0;
  int dead = 0;
  int oscillation = 0;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      if (secondGen[i][j] != firstGen[i][j]) {
        stable++;
      }
      if (newArray[i][j] == 'X') {
        dead++;
      }
      if (currentGen[i][j] != secondGen[i][j]) {
        oscillation++;
      }
    }
  }
  if (dead == 0) {
    cout << "dead" << endl;
    return true;
  }
  else if (stable == 0) {
    cout << "stable" << endl;
    return true;
  }
  else if (oscillation == 0) {
    cout << "oscillating" << endl;
    return true;
  }
  else {
    return false;
  }

}

void Calculator::CentralChunk() {
  for (int i = 1; i < rows - 1; ++i) {
    for (int j = 1; j < columns - 1; ++j) {
      numLocations = 0;
      if (newArray[i+1][j] == 'X') {
        numLocations++;
      }
      if (newArray[i+1][j-1] == 'X') {
        numLocations++;
      }
      if (newArray[i+1][j+1] == 'X') {
        numLocations++;
      }
      if (newArray[i][j+1] == 'X') {
        numLocations++;
      }
      if (newArray[i][j-1] == 'X') {
        numLocations++;
      }
      if (newArray[i-1][j] == 'X') {
        numLocations++;
      }
      if (newArray[i-1][j-1] == 'X') {
        numLocations++;
      }
      if (newArray[i-1][j+1] == 'X') {
        numLocations++;
      }
      currentGen[i][j] = numLocations;
    }
  }
}

void Calculator::EdgeCalculator() {
  currentGen[0][0] = TopLeft();
  currentGen[0][columns-1] = TopRight();
  currentGen[rows-1][0] = BottomLeft();
  currentGen[rows-1][columns-1] = BottomRight();
  for (int j = 1; j < columns - 1; ++j) {
    currentGen[0][j] = TopRow(j);
    currentGen[rows-1][j] = BottomRow(j);
  }

  for (int i = 1; i < rows - 1; ++i) {
    currentGen[i][0] = LeftSide(i);
    currentGen[i][columns-1] = RightSide(i);
  }
}

int Calculator::TopLeft() {
  int topLeftNeighbors = 0;

  if (newArray[0][1] == 'X')
  {
    topLeftNeighbors++;
  }
  if (newArray[1][1] == 'X')
  {
    topLeftNeighbors++;
  }
  if (newArray[1][0] == 'X')
  {
    topLeftNeighbors++;
  }
  return topLeftNeighbors;
}

int Calculator::TopRight() {
  int topRightNeighbors = 0;

  if (newArray[0][columns-2] == 'X')
  {
    topRightNeighbors++;
  }
  if (newArray[1][columns-2] == 'X')
  {
    topRightNeighbors++;
  }
  if (newArray[1][columns-1] == 'X')
  {
    topRightNeighbors++;
  }
  return topRightNeighbors;
}

int Calculator::BottomLeft() {
  int bottomLeftNeighbors = 0;

  if (newArray[rows-2][0] == 'X')
  {
    bottomLeftNeighbors++;
  }
  if (newArray[rows-2][1] == 'X')
  {
    bottomLeftNeighbors++;
  }
  if (newArray[rows-1][1] == 'X')
  {
    bottomLeftNeighbors++;
  }
  return bottomLeftNeighbors;
}

int Calculator::BottomRight() {
  int bottomRightNeighbors = 0;

  if (newArray[rows-1][columns-2] == 'X')
  {
    bottomRightNeighbors++;
  }
  if (newArray[rows-2][columns-2] == 'X')
  {
    bottomRightNeighbors++;
  }
  if (newArray[rows-2][columns-1] == 'X')
  {
    bottomRightNeighbors++;
  }
  return bottomRightNeighbors;
}

int Calculator::TopRow(int j) {
  int neighborCount = 0;
  if (newArray[0][j-1] == 'X') {
    neighborCount++;
  }
  if (newArray[1][j-1] == 'X') {
    neighborCount++;
  }
  if (newArray[1][j] == 'X') {
    neighborCount++;
  }
  if (newArray[1][j+1] == 'X') {
    neighborCount++;
  }
  if (newArray[0][j+1] == 'X') {
    neighborCount++;
  }
  return neighborCount;
}

int Calculator::BottomRow(int j) {
  int neighborCount = 0;
  if (newArray[rows-1][j-1] == 'X') {
    neighborCount++;
  }
  if (newArray[rows-2][j-1] == 'X') {
    neighborCount++;
  }
  if (newArray[rows-2][j] == 'X') {
    neighborCount++;
  }
  if (newArray[rows-2][j+1] == 'X') {
    neighborCount++;
  }
  if (newArray[rows-1][j+1] == 'X') {
    neighborCount++;
  }
  return neighborCount;
}

int Calculator::LeftSide(int i) {
  int neighborCount = 0;
  if (newArray[i-1][0] == 'X') {
    neighborCount++;
  }
  if (newArray[i-1][1] == 'X') {
    neighborCount++;
  }
  if (newArray[i][1] == 'X') {
    neighborCount++;
  }
  if (newArray[i+1][1] == 'X') {
    neighborCount++;
  }
  if (newArray[i+1][0] == 'X') {
    neighborCount++;
  }
  return neighborCount;
}

int Calculator::RightSide(int i) {
  int neighborCount = 0;
  if (newArray[i-1][columns-1] == 'X') {
    neighborCount++;
  }
  if (newArray[i-1][columns-2] == 'X') {
    neighborCount++;
  }
  if (newArray[i][columns-2] == 'X') {
    neighborCount++;
  }
  if (newArray[i+1][columns-2] == 'X') {
    neighborCount++;
  }
  if (newArray[i+1][columns-1] == 'X') {
    neighborCount++;
  }
  return neighborCount;
}

void Calculator::MirrorModeCalculator() {
  CentralChunk();
  M_EdgeCalculator();
  IncrementTotalNeighbors();
  CreateNewGeneration();
  MoveGenerationsDown();
  generationNumber++;
}

void Calculator::M_EdgeCalculator() {
  currentGen[0][0] = M_TopLeft();
  currentGen[0][columns-1] = M_TopRight();
  currentGen[rows-1][0] = M_BottomLeft();
  currentGen[rows-1][columns-1] = M_BottomRight();
  for (int j = 1; j < columns - 1; ++j) {
    currentGen[0][j] = M_TopRow(j);
    currentGen[rows-1][j] = M_BottomRow(j);
  }

  for (int i = 1; i < rows - 1; ++i) {
    currentGen[i][0] = M_LeftSide(i);
    currentGen[i][columns-1] = M_RightSide(i);
  }
}

int Calculator::M_TopLeft() {
  int topLeftNeighbors = 0;

  if (newArray[0][0] == 'X') {
    topLeftNeighbors += 3;
  }
  if (newArray[0][1] == 'X')
  {
    topLeftNeighbors += 2;
  }
  if (newArray[1][1] == 'X')
  {
    topLeftNeighbors++;
  }
  if (newArray[1][0] == 'X')
  {
    topLeftNeighbors += 2;
  }
  return topLeftNeighbors;
}

int Calculator::M_TopRight() {
  int topRightNeighbors = 0;

  if (newArray[0][columns-1] == 'X') {
    topRightNeighbors += 3;
  }
  if (newArray[0][columns-2] == 'X')
  {
    topRightNeighbors += 2;
  }
  if (newArray[1][columns-2] == 'X')
  {
    topRightNeighbors++;
  }
  if (newArray[1][columns-1] == 'X')
  {
    topRightNeighbors += 2;
  }
  return topRightNeighbors;
}


int Calculator::M_BottomLeft() {
  int bottomLeftNeighbors = 0;

  if (newArray[rows-1][0] == 'X') {
    bottomLeftNeighbors += 3;
  }
  if (newArray[rows-2][0] == 'X')
  {
    bottomLeftNeighbors += 2;
  }
  if (newArray[rows-2][1] == 'X')
  {
    bottomLeftNeighbors++;
  }
  if (newArray[rows-1][1] == 'X')
  {
    bottomLeftNeighbors += 2;
  }
  return bottomLeftNeighbors;
}

int Calculator::M_BottomRight() {
  int bottomRightNeighbors = 0;

  if (newArray[rows-1][columns-1] == 'X') {
    bottomRightNeighbors += 3;
  }
  if (newArray[rows-1][columns-2] == 'X')
  {
    bottomRightNeighbors += 2;
  }
  if (newArray[rows-2][columns-2] == 'X')
  {
    bottomRightNeighbors++;
  }
  if (newArray[rows-2][columns-1] == 'X')
  {
    bottomRightNeighbors += 2;
  }
  return bottomRightNeighbors;
}

int Calculator::M_TopRow(int j) {
  int neighborCount = 0;

  if (newArray[0][j] == 'X') {
    neighborCount++;
  }
  if (newArray[0][j-1] == 'X') {
    neighborCount += 2;
  }
  if (newArray[1][j-1] == 'X') {
    neighborCount++;
  }
  if (newArray[1][j] == 'X') {
    neighborCount++;
  }
  if (newArray[1][j+1] == 'X') {
    neighborCount++;
  }
  if (newArray[0][j+1] == 'X') {
    neighborCount += 2;
  }
  return neighborCount;
}

int Calculator::M_BottomRow(int j) {
  int neighborCount = 0;

  if (newArray[rows-1][j] == 'X') {
    neighborCount++;
  }
  if (newArray[rows-1][j-1] == 'X') {
    neighborCount += 2;
  }
  if (newArray[rows-2][j-1] == 'X') {
    neighborCount++;
  }
  if (newArray[rows-2][j] == 'X') {
    neighborCount++;
  }
  if (newArray[rows-2][j+1] == 'X') {
    neighborCount++;
  }
  if (newArray[rows-1][j+1] == 'X') {
    neighborCount += 2;
  }
  return neighborCount;
}

int Calculator::M_LeftSide(int i) {
  int neighborCount = 0;
  if (newArray[i][0] == 'X') {
    neighborCount++;
  }
  if (newArray[i-1][0] == 'X') {
    neighborCount += 2;
  }
  if (newArray[i-1][1] == 'X') {
    neighborCount++;
  }
  if (newArray[i][1] == 'X') {
    neighborCount++;
  }
  if (newArray[i+1][1] == 'X') {
    neighborCount++;
  }
  if (newArray[i+1][0] == 'X') {
    neighborCount += 2;
  }
  return neighborCount;
}

int Calculator::M_RightSide(int i) {
  int neighborCount = 0;

  if (newArray[i][columns-1] == 'X') {
    neighborCount++;
  }
  if (newArray[i-1][columns-1] == 'X') {
    neighborCount += 2;
  }
  if (newArray[i-1][columns-2] == 'X') {
    neighborCount++;
  }
  if (newArray[i][columns-2] == 'X') {
    neighborCount++;
  }
  if (newArray[i+1][columns-2] == 'X') {
    neighborCount++;
  }
  if (newArray[i+1][columns-1] == 'X') {
    neighborCount += 2;
  }
  return neighborCount;
}

int Calculator::GetNumbers(int maxNum) {
  return abs(rand() %  maxNum - 1);
}

void Calculator::DoughnutModeCalculator() {
  CentralChunk();
  D_EdgeCalculator();
  IncrementTotalNeighbors();
  CreateNewGeneration();
  MoveGenerationsDown();
  generationNumber++;
}

void Calculator::D_EdgeCalculator() {
  currentGen[0][0] = D_TopLeft();
  currentGen[0][columns-1] = D_TopRight();
  currentGen[rows-1][0] = D_BottomLeft();
  currentGen[rows-1][columns-1] = D_BottomRight();
  for (int j = 1; j < columns - 1; ++j) {
    currentGen[0][j] = D_TopRow(j);
    currentGen[rows-1][j] = D_BottomRow(j);
  }

  for (int i = 1; i < rows - 1; ++i) {
    currentGen[i][0] = D_LeftSide(i);
    currentGen[i][columns-1] = D_RightSide(i);
  }
}

int Calculator::D_TopLeft() {
  int topLeftNeighbors = 0;

  if (newArray[0][columns-1] == 'X') {
    topLeftNeighbors++;
  }
  if (newArray[1][columns-1] == 'X') {
    topLeftNeighbors++;
  }
  if (newArray[rows-1][0] == 'X') {
    topLeftNeighbors++;
  }
  if (newArray[rows-1][1] == 'X') {
    topLeftNeighbors++;
  }
  if (newArray[rows-1][columns-1] == 'X') {
    topLeftNeighbors++;
  }
  if (newArray[0][1] == 'X')
  {
    topLeftNeighbors++;
  }
  if (newArray[1][1] == 'X')
  {
    topLeftNeighbors++;
  }
  if (newArray[1][0] == 'X')
  {
    topLeftNeighbors++;
  }
  return topLeftNeighbors;
}

int Calculator::D_TopRight() {
  int topRightNeighbors = 0;

  if (newArray[rows-1][columns-2] == 'X') {
    topRightNeighbors++;
  }
  if (newArray[rows-1][columns-1] == 'X') {
    topRightNeighbors++;
  }
  if (newArray[rows-1][0] == 'X') {
    topRightNeighbors++;
  }
  if (newArray[1][0] == 'X') {
    topRightNeighbors++;
  }
  if (newArray[0][0] == 'X') {
    topRightNeighbors++;
  }
  if (newArray[0][columns-2] == 'X')
  {
    topRightNeighbors++;
  }
  if (newArray[1][columns-2] == 'X')
  {
    topRightNeighbors++;
  }
  if (newArray[1][columns-1] == 'X')
  {
    topRightNeighbors++;
  }
  return topRightNeighbors;
}

int Calculator::D_BottomLeft() {
  int bottomLeftNeighbors = 0;

  if (newArray[rows-1][columns-1] == 'X') {
    bottomLeftNeighbors++;
  }
  if (newArray[rows-2][columns-1] == 'X') {
    bottomLeftNeighbors++;
  }
  if (newArray[0][columns-1] == 'X') {
    bottomLeftNeighbors++;
  }
  if (newArray[0][1] == 'X') {
    bottomLeftNeighbors++;
  }
  if (newArray[0][0] == 'X') {
    bottomLeftNeighbors++;
  }
  if (newArray[rows-2][0] == 'X')
  {
    bottomLeftNeighbors++;
  }
  if (newArray[rows-2][1] == 'X')
  {
    bottomLeftNeighbors++;
  }
  if (newArray[rows-1][1] == 'X')
  {
    bottomLeftNeighbors++;
  }
  return bottomLeftNeighbors;
}

int Calculator::D_BottomRight() {
  int bottomRightNeighbors = 0;

  if (newArray[0][0] == 'X') {
    bottomRightNeighbors++;
  }
  if (newArray[0][columns-2] == 'X') {
    bottomRightNeighbors++;
  }
  if (newArray[0][columns-1] == 'X') {
    bottomRightNeighbors++;
  }
  if (newArray[rows-2][0] == 'X') {
    bottomRightNeighbors++;
  }
  if (newArray[rows-1][0] == 'X') {
    bottomRightNeighbors++;
  }
  if (newArray[rows-1][columns-2] == 'X')
  {
    bottomRightNeighbors++;
  }
  if (newArray[rows-2][columns-2] == 'X')
  {
    bottomRightNeighbors++;
  }
  if (newArray[rows-2][columns-1] == 'X')
  {
    bottomRightNeighbors++;
  }
  return bottomRightNeighbors;
}

int Calculator::D_TopRow(int j) {
  int neighborCount = 0;

  if (newArray[rows-1][j+1] == 'X') {
    neighborCount++;
  }
  if (newArray[rows-1][j-1] == 'X') {
    neighborCount++;
  }
  if (newArray[rows-1][j] == 'X') {
    neighborCount++;
  }
  if (newArray[0][j-1] == 'X') {
    neighborCount++;
  }
  if (newArray[1][j-1] == 'X') {
    neighborCount++;
  }
  if (newArray[1][j] == 'X') {
    neighborCount++;
  }
  if (newArray[1][j+1] == 'X') {
    neighborCount++;
  }
  if (newArray[0][j+1] == 'X') {
    neighborCount++;
  }
  return neighborCount;
}

int Calculator::D_BottomRow(int j) {
  int neighborCount = 0;

  if (newArray[0][j] == 'X') {
    neighborCount++;
  }
  if (newArray[0][j-1] == 'X') {
    neighborCount++;
  }
  if (newArray[0][j+1] == 'X') {
    neighborCount++;
  }
  if (newArray[rows-1][j-1] == 'X') {
    neighborCount++;
  }
  if (newArray[rows-2][j-1] == 'X') {
    neighborCount++;
  }
  if (newArray[rows-2][j] == 'X') {
    neighborCount++;
  }
  if (newArray[rows-2][j+1] == 'X') {
    neighborCount++;
  }
  if (newArray[rows-1][j+1] == 'X') {
    neighborCount++;
  }
  return neighborCount;
}

int Calculator::D_LeftSide(int i) {
  int neighborCount = 0;

  if (newArray[i][columns-1] == 'X') {
    neighborCount++;
  }
  if (newArray[i+1][columns-1] == 'X') {
    neighborCount++;
  }
  if (newArray[i-1][columns-1] == 'X') {
    neighborCount++;
  }
  if (newArray[i-1][0] == 'X') {
    neighborCount++;
  }
  if (newArray[i-1][1] == 'X') {
    neighborCount++;
  }
  if (newArray[i][1] == 'X') {
    neighborCount++;
  }
  if (newArray[i+1][1] == 'X') {
    neighborCount++;
  }
  if (newArray[i+1][0] == 'X') {
    neighborCount++;
  }
  return neighborCount;
}

int Calculator::D_RightSide(int i) {
  int neighborCount = 0;

  if (newArray[i][0] == 'X') {
    neighborCount++;
  }
  if (newArray[i+1][0] == 'X') {
    neighborCount++;
  }
  if (newArray[i-1][0] == 'X') {
    neighborCount++;
  }
  if (newArray[i-1][columns-1] == 'X') {
    neighborCount++;
  }
  if (newArray[i-1][columns-2] == 'X') {
    neighborCount++;
  }
  if (newArray[i][columns-2] == 'X') {
    neighborCount++;
  }
  if (newArray[i+1][columns-2] == 'X') {
    neighborCount++;
  }
  if (newArray[i+1][columns-1] == 'X') {
    neighborCount++;
  }
  return neighborCount;
}

void Calculator::PrintToConsole() {
  cout << "Generation number: " << generationNumber << endl;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      cout << newArray[i][j];
    }
    cout << endl;
  }
  cout << endl;
}

void Calculator::OpenOut(string fileOutput) {
  outFS.open(fileOutput, fstream::app);
}

char** Calculator::GetNewArray() {
  return newArray;
}

void Calculator::PrintToFile() {
  outFS << "Generation number: " << generationNumber << endl;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      outFS << newArray[i][j];
    }
    outFS << endl;
  }
  outFS << endl;
}

void Calculator::CloseOut()
{
  outFS.close();
}
