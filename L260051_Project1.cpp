/*
    CS1002 Programming Fundamentals - Project 1
    Console Image Editor

    This file has three parts:
      Part A: functions that read and save image files (already done)
      Part B: YOUR functions (complete every function marked TODO)
      Part C: the menu and main() (already done)

    Only write code in Part B. Do not change Part A or Part C.

    You do not need to understand the Part A code.
*/

#include <iostream>
#include <fstream>
using namespace std;

const int MAX_SIZE = 1000; // biggest image allowed is 1000 x 1000
const int NAME_LEN = 100;  // longest file name allowed

/* =====================================================================
   PART A: PROVIDED FUNCTIONS (do not change)
   ===================================================================== */

// Returns true if the file name ends with ".pgm" (small or capital letters).
bool isPGMFile(const char name[])
{
    int len = 0;
    while (name[len] != '\0') // find the length of the name
        len++;

    if (len < 5) // shortest valid name is "a.pgm"
        return false;

    char ext[5] = ".pgm";
    for (int i = 0; i < 4; i++)
    {
        char ch = name[len - 4 + i];
        if (ch >= 'A' && ch <= 'Z') // change a capital letter to small
            ch = ch + 32;
        if (ch != ext[i])
            return false;
    }
    return true;
}

// Reads a PGM file into the 2D array.
// After loading, image[r][c] is the pixel at row r and column c.
// Every pixel is from 0 (black) to 255 (white).
bool loadPGM(const char fileName[], int image[][MAX_SIZE], int &rows, int &cols)
{
    ifstream fin(fileName);
    if (!fin)
    {
        cout << "Could not open " << fileName << ". Check the name and the folder.\n";
        return false;
    }

    // Step 1: the file must start with P2
    char ch1, ch2;
    fin >> ch1 >> ch2;
    if (ch1 != 'P' || ch2 != '2')
    {
        cout << "This is not a plain PGM file (it must start with P2).\n";
        return false;
    }

    // Step 2: skip comment lines (they start with #)
    fin >> ws;
    while (fin.peek() == '#')
    {
        fin.ignore(1000, '\n');
        fin >> ws;
    }

    // Step 3: read width, height and the largest gray value
    // NOTE: the file gives the WIDTH (columns) first and then the HEIGHT (rows)
    int width, height, maxValue;
    fin >> width >> height >> maxValue;
    if (!fin || width < 1 || height < 1 || width > MAX_SIZE || height > MAX_SIZE || maxValue < 1)
    {
        cout << "This image size is not supported.\n";
        return false;
    }

    // Step 4: read the pixels row by row
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            int value;
            fin >> value;
            image[r][c] = value * 255 / maxValue; // store every pixel as 0 to 255
        }
    }
    if (!fin)
    {
        cout << "The file ended too early.\n";
        return false;
    }

    rows = height;
    cols = width;
    cout << "Loaded " << fileName << " (width " << cols << ", height " << rows << ")\n";
    return true;
}

// Writes the 2D array to a PGM file.
bool savePGM(const char fileName[], int image[][MAX_SIZE], int rows, int cols)
{
    ofstream fout(fileName);
    if (!fout)
    {
        cout << "Could not create " << fileName << "\n";
        return false;
    }

    fout << "P2\n";
    fout << cols << " " << rows << "\n"; // width first, then height
    fout << 255 << "\n";

    // write the pixels row by row, 20 numbers on each line
    int count = 0;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            fout << image[r][c] << " ";
            count++;
            if (count == 20)
            {
                fout << "\n";
                count = 0;
            }
        }
    }
    if (count != 0)
        fout << "\n";

    fout.close();
    return true;
}

/* =====================================================================
   PART B: YOUR FUNCTIONS
   Complete every function marked TODO. You may add extra functions.
   The return statements are only there so that the program compiles.

   NOTE: an array is never copied when it is passed to a function. The
   function works on the same array that main() has, so when you change
   image[r][c] inside a function, the picture in main() changes too.
   That is why "image" has no &, but "rows" and "cols" need & when a
   function has to change them.
   ===================================================================== */

// Task 1: Add "amount" to every pixel.
// If a pixel goes below 0 make it 0. If it goes above 255 make it 255.
void changeBrightness(int image[][MAX_SIZE], int rows, int cols, int n)
{

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int value = image[i][j] + n;
            if (value > 255)
            {
                value = 255;
            }
            else if (value < 0)
            {
                value = 0;
            }
            image[i][j] = value;
        }
    }
}

// Task 2: Every pixel p becomes 255 - p.
void makeNegative(int image[][MAX_SIZE], int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            image[i][j] = 255 - image[i][j];
        }
    }
}

// Task 3: A pixel that is >= limit becomes 255 (white). Every other pixel becomes 0 (black).
void makeBlackWhite(int image[][MAX_SIZE], int rows, int cols, int limit)
{
    // TODO
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int value = image[i][j];
            if (value >= limit)
            {
                value = 255;
            }
            else if (value < limit)
            {
                value = 0;
            }
            image[i][j] = value;
        }
    }
}

// Task 4: Swap the values of a and b.
void swapValues(int &a, int &b)
{
    int c = a;
    a = b;
    b = c;
}

// Task 4: Mirror the image (left side goes to the right side).
void flipHorizontal(int image[][MAX_SIZE], int rows, int cols)
{
    for (int i = 0; i < rows / 2; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            swapValues(image[i][j], image[rows - i - 1][j]);
        }
    }
}

// Task 4: Turn the image upside down.
void flipVertical(int image[][MAX_SIZE], int rows, int cols)
{
    // Vertical
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols / 2; j++)
        {
            swapValues(image[i][j], image[i][cols - j - 1]);
        }
    }
}

// Task 5: Copy rows x cols pixels from the array "from" into the array "to".
void copyImage(int from[][MAX_SIZE], int to[][MAX_SIZE], int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            to[i][j] = from[i][j];
        }
    }
}

// Task 5: Rotate 90 degrees clockwise.
// Write the rotated picture into temp, copy it back into image, then swap rows and cols.
void rotateClockwise(int image[][MAX_SIZE], int temp[][MAX_SIZE], int &rows, int &cols)
{

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            temp[i][j] = image[rows - 1 - j][i];
        }
    }
    copyImage(temp, image, rows, cols);
}

// Task 5: Rotate 90 degrees anti-clockwise.
void rotateAntiClockwise(int image[][MAX_SIZE], int temp[][MAX_SIZE], int &rows, int &cols)
{
    // TODO
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            temp[i][j] = image[j][cols - 1 - i];
        }
    }
    copyImage(temp, image, rows, cols);
}

// Task 5: Rotate 180 degrees. Do NOT write any loop here. Call your other functions.
void rotate180(int image[][MAX_SIZE], int temp[][MAX_SIZE], int &rows, int &cols)
{
    // TODO
    flipHorizontal(image, rows, cols);
    flipVertical(image, rows, cols);
}

// Task 6: Blur. Every pixel (except the border) becomes the average of
// the 3 x 3 block around it. Border pixels do not change.
void blurImage(int image[][MAX_SIZE], int temp[][MAX_SIZE], int rows, int cols)
{

    copyImage(image, temp, rows, cols);
    int three[3][3] = {};
    int n = 3;
    int start_r = 0;
    while (start_r < rows - 2)
    {
        int start_c = 0;
        while (start_c < cols - 2)
        {
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    three[i][j] = image[i + start_r][j + start_c];
                }
            }
            int sum = 0;
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    sum += three[i][j];
                }
            }
            int avg = sum / 9;
            temp[1 + start_r][1 + start_c] = avg;
            start_c++;
        }
        start_r++;
    }
    copyImage(temp, image, rows, cols);
}

// Task 7: Sort arr[0] to arr[n-1] in ascending order. Return the number of comparisons between two array elements.
int sortArray(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swapValues(arr[j], arr[j + 1]);
            }
        }
    }
    return 0;
}

// Task 7: Remove noise. Every pixel (except the border) becomes the middle
// value of its sorted 3 x 3 block. Return the total comparisons made by sortArray.
int removeNoise(int image[][MAX_SIZE], int temp[][MAX_SIZE], int rows, int cols)
{

    copyImage(image, temp, rows, cols);

    int three[3][3] = {};
    int n = 3;
    int start_r = 0;
    int straight[9] = {};
    while (start_r < rows - 2)
    {
        int start_c = 0;
        while (start_c < cols - 2)
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    three[i][j] = image[start_r + i][start_c + j];
                }
            }
            int count = 0;
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    straight[count] = three[i][j];
                    count++;
                }
            }

            sortArray(straight, count);

            int value = straight[4];
            temp[start_r + 1][start_c + 1] = value;

            start_c++;
        }
        start_r++;
    }

    copyImage(temp, image, rows, cols);
    return 0;
}

void findEdges(int image[][MAX_SIZE], int temp[][MAX_SIZE], int rows, int cols)
{
    copyImage(image, temp, rows, cols); // temp mein original

    // image ko 0 kar do, border automatically 0 rahega
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            image[i][j] = 0;
        }
    }

    int block[3][3];

    for (int r = 1; r < rows - 1; r++)
    {
        for (int c = 1; c < cols - 1; c++)
        {
            // 3x3 block temp se copy
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    block[i][j] = temp[r - 1 + i][c - 1 + j];
                }
            }

            int gx = (block[0][2] + 2 * block[1][2] + block[2][2]) - (block[0][0] + 2 * block[1][0] + block[2][0]);

            int gy = (block[2][0] + 2 * block[2][1] + block[2][2]) - (block[0][0] + 2 * block[0][1] + block[0][2]);

            if (gx < 0)
            {
                gx = -gx;
            }
            if (gy < 0)
            {
                gy = -gy;
            }

            int val = gx + gy;
            if (val > 255)
            {
                val = 255;
            }

            image[r][c] = val;
        }
    }
}

/* =====================================================================
   PART C: MENU AND MAIN (do not change)
   ===================================================================== */

// Keeps asking for a file name until an image is loaded.
void askAndLoad(int image[][MAX_SIZE], int &rows, int &cols)
{
    char fileName[NAME_LEN];
    bool loaded = false;
    while (!loaded && cin)
    {
        cout << "Enter the image file name (example: coins.ascii.pgm): ";
        cin >> fileName;
        if (isPGMFile(fileName))
            loaded = loadPGM(fileName, image, rows, cols);
        else
            cout << "The name must end with .pgm\n";
    }
}

// Asks for a file name and saves the image.
void saveResult(int image[][MAX_SIZE], int rows, int cols)
{
    char fileName[NAME_LEN];
    cout << "Save the result as (example: result.pgm): ";
    cin >> fileName;
    while (!isPGMFile(fileName) && cin)
    {
        cout << "The name must end with .pgm. Try again: ";
        cin >> fileName;
    }
    if (savePGM(fileName, image, rows, cols))
        cout << "Saved. Open " << fileName << " in the PGM viewer to check it.\n";
}

void showMenu()
{
    cout << "\n------------------- MENU -------------------\n";
    cout << " 1. Change brightness\n";
    cout << " 2. Negative\n";
    cout << " 3. Black and white\n";
    cout << " 4. Flip\n";
    cout << " 5. Rotate\n";
    cout << " 6. Blur\n";
    cout << " 7. Remove noise\n";
    cout << " 8. Find edges (bonus)\n";
    cout << " 9. Load another image\n";
    cout << " 0. Exit\n";
    cout << "Enter your choice: ";
}

int main()
{
    // These arrays are very big (about 4 MB each), so they are "static".
    // Without "static" the program may crash. Do not change this.
    static int image[MAX_SIZE][MAX_SIZE]; // the picture
    static int temp[MAX_SIZE][MAX_SIZE];  // spare array for rotate, blur and remove noise
    int rows = 0, cols = 0;

    cout << "========= PF Project 1: Image Editor =========\n";
    askAndLoad(image, rows, cols);

    // NOTE: every option changes the current picture, so changes add up.
    // Use option 9 to start again from a file.
    int choice = -1;
    while (choice != 0 && cin)
    {
        showMenu();
        cin >> choice;
        if (!cin) // the input was not a number
            break;

        bool changed = true; // did the picture change? then save it

        switch (choice)
        {
        case 1:
        {
            int amount;
            cout << "Enter the amount (-255 to 255): ";
            cin >> amount;
            changeBrightness(image, rows, cols, amount);
            break;
        }
        case 2:
            makeNegative(image, rows, cols);
            break;
        case 3:
        {
            int limit;
            cout << "Enter the limit (0 to 255): ";
            cin >> limit;
            makeBlackWhite(image, rows, cols, limit);
            break;
        }
        case 4:
        {
            int type;
            cout << "1. Left-right   2. Upside down: ";
            cin >> type;
            if (type == 1)
                flipHorizontal(image, rows, cols);
            else if (type == 2)
                flipVertical(image, rows, cols);
            else
                changed = false;
            break;
        }
        case 5:
        {
            int type;
            cout << "1. 90 clockwise   2. 90 anti-clockwise   3. 180: ";
            cin >> type;
            if (type == 1)
                rotateClockwise(image, temp, rows, cols);
            else if (type == 2)
                rotateAntiClockwise(image, temp, rows, cols);
            else if (type == 3)
                rotate180(image, temp, rows, cols);
            else
                changed = false;
            cout << "Size is now: width " << cols << ", height " << rows << "\n";
            break;
        }
        case 6:
            blurImage(image, temp, rows, cols);
            break;
        case 7:
            cout << "Comparisons made while sorting: "
                 << removeNoise(image, temp, rows, cols) << "\n";
            break;
        case 8:
            findEdges(image, temp, rows, cols);
            break;
        case 9:
            askAndLoad(image, rows, cols);
            changed = false;
            break;
        case 0:
            changed = false;
            break;
        default:
            cout << "Invalid choice.\n";
            changed = false;
        }

        if (changed)
            saveResult(image, rows, cols);
    }

    cout << "Goodbye!\n";
    return 0;
}
