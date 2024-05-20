#include <windows.h>
#include <winsqlite/winsqlite3.h>
#include <string>
#include <sstream>

#define ID_LOGIN 1
#define ID_LOGIN_BUTTON 6
#define ID_SIGNUP 2
#define ID_SIGNUP_BUTTON 7
#define ID_CHECK_BALANCE 3
#define ID_TRANSFER 4
#define ID_NOTICE 5
#define ID_MENU 8
#define ID_EXIT 9

HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

struct LoginSignupData {
    HWND hUsername;
    HWND hPassword;
};

void HandleLogin(HWND hwnd);
void HandleSignup(HWND hwnd);
void HandleCheckBalance(HWND hwnd);
void HandleTransfer(HWND hwnd);
void HandleNotice(HWND hwnd);
void CreateLoginDialog(HWND hwnd);
void CreateSignupDialog(HWND hwnd);
void CreateMainMenu(HWND hwnd);

void ClearWindowControls(HWND hwnd) {
    HWND hChild = GetWindow(hwnd, GW_CHILD);
    while (hChild) {
        DestroyWindow(hChild);
        hChild = GetWindow(hwnd, GW_CHILD);
    }
}

void CreateLoginDialog(HWND hwnd) {
    // Clear existing controls
    ClearWindowControls(hwnd);

    // Create username and password input fields
    CreateWindow(L"static", L"Username:", WS_VISIBLE | WS_CHILD, 50, 50, 80, 25, hwnd, NULL, hInst, NULL);
    HWND hUsername = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 50, 150, 25, hwnd, NULL, hInst, NULL);

    CreateWindow(L"static", L"Password:", WS_VISIBLE | WS_CHILD, 50, 100, 80, 25, hwnd, NULL, hInst, NULL);
    HWND hPassword = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 150, 100, 150, 25, hwnd, NULL, hInst, NULL);

    // Create login button
    CreateWindow(L"button", L"Login", WS_VISIBLE | WS_CHILD, 150, 150, 80, 25, hwnd, (HMENU)ID_LOGIN_BUTTON, hInst, NULL);

    // Store the handles for username and password fields in the window's user data
    LoginSignupData* data = new LoginSignupData{ hUsername, hPassword };
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)data);
}

void CreateSignupDialog(HWND hwnd) {
    // Clear existing controls
    ClearWindowControls(hwnd);

    // Create username and password input fields
    CreateWindow(L"static", L"Username:", WS_VISIBLE | WS_CHILD, 50, 50, 80, 25, hwnd, NULL, hInst, NULL);
    HWND hUsername = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 50, 150, 25, hwnd, NULL, hInst, NULL);

    CreateWindow(L"static", L"Password:", WS_VISIBLE | WS_CHILD, 50, 100, 80, 25, hwnd, NULL, hInst, NULL);
    HWND hPassword = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 150, 100, 150, 25, hwnd, NULL, hInst, NULL);

    // Create signup button
    CreateWindow(L"button", L"Signup", WS_VISIBLE | WS_CHILD, 150, 150, 80, 25, hwnd, (HMENU)ID_SIGNUP_BUTTON, hInst, NULL);

    // Store the handles for username and password fields in the window's user data
    LoginSignupData* data = new LoginSignupData{ hUsername, hPassword };
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)data);
}

void HandleLogin(HWND hwnd) {
    // Retrieve the handles for username and password fields
    LoginSignupData* data = (LoginSignupData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    HWND hUsername = data->hUsername;
    HWND hPassword = data->hPassword;

    // Get the username and password entered by the user
    wchar_t username[50], password[50];
    GetWindowTextW(hUsername, username, 50);
    GetWindowTextW(hPassword, password, 50);

    // Convert username and password to std::string
    std::wstring wUsername(username), wPassword(password);
    std::string sUsername(wUsername.begin(), wUsername.end());
    std::string sPassword(wPassword.begin(), wPassword.end());

    // Check if username or password is empty
    if (sUsername.empty() || sPassword.empty()) {
        MessageBox(NULL, L"Username and password cannot be empty", L"Error", MB_OK);
        return;
    }

    // Open the database
    sqlite3* db;
    sqlite3_open("bank.db", &db);

    // Prepare the SQL statement
    std::stringstream ss;
    ss << "SELECT * FROM users WHERE username='" << sUsername << "' AND password='" << sPassword << "';";
    std::string sql = ss.str();

    // Execute the SQL statement
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        MessageBox(NULL, L"Failed to execute query", L"Error", MB_OK);
        sqlite3_close(db);
        return;
    }

    // Check if a matching user was found
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        MessageBox(NULL, L"Login successful!", L"Success", MB_OK);
        // Transition to main menu screen
        CreateMainMenu(hwnd);
    }
    else {
        MessageBox(NULL, L"Invalid username or password", L"Error", MB_OK);
    }

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void HandleSignup(HWND hwnd) {
    // Retrieve the handles for username and password fields
    LoginSignupData* data = (LoginSignupData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    HWND hUsername = data->hUsername;
    HWND hPassword = data->hPassword;

    // Get the username and password entered by the user
    wchar_t username[50], password[50];
    GetWindowTextW(hUsername, username, 50);
    GetWindowTextW(hPassword, password, 50);

    // Convert username and password to std::string
    std::wstring wUsername(username), wPassword(password);
    std::string sUsername(wUsername.begin(), wUsername.end());
    std::string sPassword(wPassword.begin(), wPassword.end());

    // Check if username or password is empty
    if (sUsername.empty() || sPassword.empty()) {
        MessageBox(NULL, L"Username and password cannot be empty", L"Error", MB_OK);
        return;
    }

    // Open the database
    sqlite3* db;
    sqlite3_open("bank.db", &db);

    // Check if the username already exists
    std::stringstream ssCheck;
    ssCheck << "SELECT * FROM users WHERE username='" << sUsername << "';";
    std::string sqlCheck = ssCheck.str();

    sqlite3_stmt* stmtCheck;
    int rc = sqlite3_prepare_v2(db, sqlCheck.c_str(), -1, &stmtCheck, 0);
    if (rc != SQLITE_OK) {
        MessageBox(NULL, L"Failed to execute query", L"Error", MB_OK);
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_step(stmtCheck);
    if (rc == SQLITE_ROW) {
        MessageBox(NULL, L"Username already exists", L"Error", MB_OK);
        sqlite3_finalize(stmtCheck);
        sqlite3_close(db);
        return;
    }
    sqlite3_finalize(stmtCheck);

    // Prepare the SQL statement to insert new user
    std::stringstream ss;
    ss << "INSERT INTO users (username, password, balance) VALUES ('" << sUsername << "', '" << sPassword << "', 0.0);";
    std::string sql = ss.str();

    // Execute the SQL statement
    char* errMsg = 0;
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        MessageBox(NULL, L"Failed to add user", L"Error", MB_OK);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return;
    }

    // Retrieve the account number
    int accountId = (int)sqlite3_last_insert_rowid(db);

    // Display the account number
    std::wstringstream ws;
    ws << L"Signup successful! Your account number is " << accountId;
    MessageBox(NULL, ws.str().c_str(), L"Success", MB_OK);

    // Clean up
    sqlite3_close(db);

    // Redirect to login screen
    CreateLoginDialog(hwnd);
}


void HandleCheckBalance(HWND hwnd) {
    // Clear existing controls
    ClearWindowControls(hwnd);

    // Create controls for checking balance
    // Implement the functionality using SQLite to check and display the balance
}

void HandleTransfer(HWND hwnd) {
    // Clear existing controls
    ClearWindowControls(hwnd);

    // Create controls for transferring money
    // Implement the transfer functionality using SQLite to update balances
}

void HandleNotice(HWND hwnd) {
    // Clear existing controls
    ClearWindowControls(hwnd);

    // Create controls for displaying notices
    // Implement functionality to display fixed rates and other notices
}

void CreateMainMenu(HWND hwnd) {
    // Clear existing controls
    ClearWindowControls(hwnd);

    // Create main menu buttons
    CreateWindow(L"button", L"Check Balance", WS_VISIBLE | WS_CHILD, 50, 50, 150, 30, hwnd, (HMENU)ID_CHECK_BALANCE, hInst, NULL);
    CreateWindow(L"button", L"Transfer Money", WS_VISIBLE | WS_CHILD, 50, 100, 150, 30, hwnd, (HMENU)ID_TRANSFER, hInst, NULL);
    CreateWindow(L"button", L"Notice", WS_VISIBLE | WS_CHILD, 50, 150, 150, 30, hwnd, (HMENU)ID_NOTICE, hInst, NULL);

    // Create Menu and Exit buttons
    CreateWindow(L"button", L"Menu", WS_VISIBLE | WS_CHILD, 50, 200, 150, 30, hwnd, (HMENU)ID_MENU, hInst, NULL);
    CreateWindow(L"button", L"Exit", WS_VISIBLE | WS_CHILD, 50, 250, 150, 30, hwnd, (HMENU)ID_EXIT, hInst, NULL);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MSG msg;
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"BankManagementSystem";

    RegisterClass(&wc);
    HWND hwnd = CreateWindow(wc.lpszClassName, L"Bank Management System", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, 0, 0, hInstance, 0);
    hInst = hInstance;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

void InitializeDatabase() {
    sqlite3* db;
    char* errMsg = 0;
    int rc = sqlite3_open("bank.db", &db);
    if (rc) {
        MessageBox(NULL, L"Can't open database", L"Error", MB_OK);
        return;
    }

    const char* sql = "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT NOT NULL, "
        "password TEXT NOT NULL, "
        "balance REAL DEFAULT 0.0);"
        "CREATE TABLE IF NOT EXISTS transactions ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "from_user INTEGER, "
        "to_user INTEGER, "
        "amount REAL, "
        "FOREIGN KEY(from_user) REFERENCES users(id), "
        "FOREIGN KEY(to_user) REFERENCES users(id));";

    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        MessageBox(NULL, L"Database initialization failed", L"Error", MB_OK);
        sqlite3_free(errMsg);
    }
    sqlite3_close(db);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        CreateWindow(L"button", L"Login", WS_VISIBLE | WS_CHILD, 50, 50, 100, 30, hwnd, (HMENU)ID_LOGIN, hInst, NULL);
        CreateWindow(L"button", L"Signup", WS_VISIBLE | WS_CHILD, 200, 50, 100, 30, hwnd, (HMENU)ID_SIGNUP, hInst, NULL);
        InitializeDatabase();
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_LOGIN:
            CreateLoginDialog(hwnd);
            break;
        case ID_LOGIN_BUTTON:
            HandleLogin(hwnd);
            break;
        case ID_SIGNUP:
            CreateSignupDialog(hwnd);
            break;
        case ID_SIGNUP_BUTTON:
            HandleSignup(hwnd);
            break;
        case ID_CHECK_BALANCE:
            HandleCheckBalance(hwnd);
            break;
        case ID_TRANSFER:
            HandleTransfer(hwnd);
            break;
        case ID_NOTICE:
            HandleNotice(hwnd);
            break;
        case ID_MENU:
            CreateMainMenu(hwnd);
            break;
        case ID_EXIT:
            PostQuitMessage(0);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
