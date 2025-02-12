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
#define ID_TRANSFER_MONEY 11
#define ID_NOTICE 5
#define ID_MENU 8
#define ID_EXIT 9
#define ID_BACK 10
#define ID_LIST_ACC 12
#define ID_LOGS 13

HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

struct LoginSignupData {
    HWND hUsername;
    HWND hPassword;
};

struct UserData {
    int userID;
    double balance;
};

UserData currentUser = { -1, 0.0 };  // Initialize with default values

void HandleLogin(HWND hwnd);
void HandleSignup(HWND hwnd);
void HandleCheckBalance(HWND hwnd);
void HandleTransfer(HWND hwnd);
void HandleNotice(HWND hwnd);
void HandleLogs(HWND hwnd);
void CreateLoginDialog(HWND hwnd);
void CreateSignupDialog(HWND hwnd);
void CreateMainMenu(HWND hwnd);
void ClearWindowControls(HWND hwnd);
void ClearScreen(HWND hwnd);
void CreateInitialScreen(HWND hwnd);
void CreateTransferDialog(HWND hwnd);

void ClearScreen(HWND hwnd) {
    HDC hdc = GetDC(hwnd);
    RECT rect;
    GetClientRect(hwnd, &rect);
    HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    FillRect(hdc, &rect, hBrush);
    ReleaseDC(hwnd, hdc);
}

void ClearWindowControls(HWND hwnd) {
    HWND hChild = GetWindow(hwnd, GW_CHILD);
    while (hChild) {
        DestroyWindow(hChild);
        hChild = GetWindow(hwnd, GW_CHILD);
    }
    ClearScreen(hwnd);
    InvalidateRect(hwnd, NULL, TRUE);
}

void CreateInitialScreen(HWND hwnd) {
    ClearWindowControls(hwnd);

    currentUser = { -1, 0.0 };

    CreateWindow(L"button", L"Login", WS_VISIBLE | WS_CHILD, 50, 50, 100, 30, hwnd, (HMENU)ID_LOGIN, hInst, NULL);
    CreateWindow(L"button", L"Signup", WS_VISIBLE | WS_CHILD, 200, 50, 100, 30, hwnd, (HMENU)ID_SIGNUP, hInst, NULL);
}

void CreateLoginDialog(HWND hwnd) {
    ClearWindowControls(hwnd);

    CreateWindow(L"static", L"Username:", WS_VISIBLE | WS_CHILD, 50, 50, 80, 25, hwnd, NULL, hInst, NULL);
    HWND hUsername = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 50, 150, 25, hwnd, NULL, hInst, NULL);

    CreateWindow(L"static", L"Password:", WS_VISIBLE | WS_CHILD, 50, 100, 80, 25, hwnd, NULL, hInst, NULL);
    HWND hPassword = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 150, 100, 150, 25, hwnd, NULL, hInst, NULL);

    CreateWindow(L"button", L"Login", WS_VISIBLE | WS_CHILD, 150, 150, 80, 25, hwnd, (HMENU)ID_LOGIN_BUTTON, hInst, NULL);

    LoginSignupData* data = new LoginSignupData{ hUsername, hPassword };
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)data);
}

void CreateSignupDialog(HWND hwnd) {
    ClearWindowControls(hwnd);

    CreateWindow(L"static", L"Username:", WS_VISIBLE | WS_CHILD, 50, 50, 80, 25, hwnd, NULL, hInst, NULL);
    HWND hUsername = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 50, 150, 25, hwnd, NULL, hInst, NULL);

    CreateWindow(L"static", L"Password:", WS_VISIBLE | WS_CHILD, 50, 100, 80, 25, hwnd, NULL, hInst, NULL);
    HWND hPassword = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 150, 100, 150, 25, hwnd, NULL, hInst, NULL);

    CreateWindow(L"button", L"Signup", WS_VISIBLE | WS_CHILD, 150, 150, 80, 25, hwnd, (HMENU)ID_SIGNUP_BUTTON, hInst, NULL);

    LoginSignupData* data = new LoginSignupData{ hUsername, hPassword };
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)data);
}

void CreateTransferDialog(HWND hwnd) {
    ClearWindowControls(hwnd);

    std::wstringstream ws;
    ws << L"Account Number: " << currentUser.userID << L"\nBalance: $" << currentUser.balance;
    CreateWindow(L"static", ws.str().c_str(), WS_VISIBLE | WS_CHILD, 50, 10, 250, 50, hwnd, NULL, hInst, NULL);

    CreateWindow(L"static", L"Recipient Account Number:", WS_VISIBLE | WS_CHILD, 50, 70, 200, 25, hwnd, NULL, hInst, NULL);
    HWND hRecipient = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 250, 70, 150, 25, hwnd, NULL, hInst, NULL);

    CreateWindow(L"static", L"Amount:", WS_VISIBLE | WS_CHILD, 50, 110, 80, 25, hwnd, NULL, hInst, NULL);
    HWND hAmount = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 250, 110, 150, 25, hwnd, NULL, hInst, NULL);

    CreateWindow(L"button", L"Transfer", WS_VISIBLE | WS_CHILD, 150, 150, 80, 25, hwnd, (HMENU)ID_TRANSFER_MONEY, hInst, NULL);
    CreateWindow(L"button", L"Available Accounts", WS_VISIBLE | WS_CHILD, 250, 150, 150, 25, hwnd, (HMENU)ID_LIST_ACC, hInst, NULL);

    CreateWindow(L"button", L"Back", WS_VISIBLE | WS_CHILD, 50, 190, 80, 30, hwnd, (HMENU)ID_BACK, hInst, NULL);
    CreateWindow(L"button", L"Exit", WS_VISIBLE | WS_CHILD, 150, 190, 80, 30, hwnd, (HMENU)ID_EXIT, hInst, NULL);

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)new LoginSignupData{ hRecipient, hAmount });
}

void HandleLogin(HWND hwnd) {
    LoginSignupData* data = (LoginSignupData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    HWND hUsername = data->hUsername;
    HWND hPassword = data->hPassword;

    wchar_t username[50], password[50];
    GetWindowTextW(hUsername, username, 50);
    GetWindowTextW(hPassword, password, 50);

    std::wstring wUsername(username), wPassword(password);
    std::string sUsername(wUsername.begin(), wUsername.end());
    std::string sPassword(wPassword.begin(), wPassword.end());

    if (sUsername.empty() || sPassword.empty()) {
        MessageBox(NULL, L"Username and password cannot be empty", L"Error", MB_OK);
        return;
    }

    sqlite3* db;
    sqlite3_open("bank.db", &db);

    std::stringstream ss;
    ss << "SELECT id, balance FROM users WHERE username='" << sUsername << "' AND password='" << sPassword << "';";
    std::string sql = ss.str();

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        MessageBox(NULL, L"Failed to execute query", L"Error", MB_OK);
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        currentUser.userID = sqlite3_column_int(stmt, 0);
        currentUser.balance = sqlite3_column_double(stmt, 1);
        MessageBox(NULL, L"Login successful!", L"Success", MB_OK);
        CreateMainMenu(hwnd);
    }
    else {
        MessageBox(NULL, L"Invalid username or password", L"Error", MB_OK);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void HandleSignup(HWND hwnd) {
    LoginSignupData* data = (LoginSignupData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    HWND hUsername = data->hUsername;
    HWND hPassword = data->hPassword;

    wchar_t username[50], password[50];
    GetWindowTextW(hUsername, username, 50);
    GetWindowTextW(hPassword, password, 50);

    std::wstring wUsername(username), wPassword(password);
    std::string sUsername(wUsername.begin(), wUsername.end());
    std::string sPassword(wPassword.begin(), wPassword.end());

    if (sUsername.empty() || sPassword.empty()) {
        MessageBox(NULL, L"Username and password cannot be empty", L"Error", MB_OK);
        return;
    }

    sqlite3* db;
    sqlite3_open("bank.db", &db);

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

    std::stringstream ss;
    ss << "INSERT INTO users (username, password, balance) VALUES ('" << sUsername << "', '" << sPassword << "', 0.0);";
    std::string sql = ss.str();

    char* errMsg = 0;
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        MessageBox(NULL, L"Failed to add user", L"Error", MB_OK);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return;
    }

    int accountId = (int)sqlite3_last_insert_rowid(db);

    std::wstringstream ws;
    ws << L"Signup successful! Your account number is " << accountId;
    MessageBox(NULL, ws.str().c_str(), L"Success", MB_OK);

    sqlite3_close(db);

    CreateLoginDialog(hwnd);
}

void HandleCheckBalance(HWND hwnd) {
    ClearWindowControls(hwnd);

    std::wstringstream ws;
    ws << L"Your balance is: $" << currentUser.balance;
    CreateWindow(L"static", ws.str().c_str(), WS_VISIBLE | WS_CHILD, 50, 50, 200, 30, hwnd, NULL, hInst, NULL);

    CreateWindow(L"button", L"Back", WS_VISIBLE | WS_CHILD, 50, 100, 80, 30, hwnd, (HMENU)ID_BACK, hInst, NULL);
    CreateWindow(L"button", L"Exit", WS_VISIBLE | WS_CHILD, 150, 100, 80, 30, hwnd, (HMENU)ID_EXIT, hInst, NULL);
}



void HandleTransfer(HWND hwnd) {
    LoginSignupData* data = (LoginSignupData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    HWND hRecipient = data->hUsername;
    HWND hAmount = data->hPassword;

    wchar_t recipient[50], amount[50];
    GetWindowTextW(hRecipient, recipient, 50);
    GetWindowTextW(hAmount, amount, 50);

    std::wstring wRecipient(recipient), wAmount(amount);
    std::string sRecipient(wRecipient.begin(), wRecipient.end());
    double dAmount = std::stod(wAmount);

    if (sRecipient.empty() || dAmount <= 0) {
        MessageBox(NULL, L"Recipient and amount cannot be empty or negative", L"Error", MB_OK);
        return;
    }

    sqlite3* db;
    if (sqlite3_open("bank.db", &db) != SQLITE_OK) {
        MessageBox(NULL, L"Failed to open database", L"Error", MB_OK);
        return;
    }

    std::stringstream ssCheck;
    ssCheck << "SELECT id FROM users WHERE id=" << sRecipient << ";";
    std::string sqlCheck = ssCheck.str();

    sqlite3_stmt* stmtCheck;
    int rc = sqlite3_prepare_v2(db, sqlCheck.c_str(), -1, &stmtCheck, 0);
    if (rc != SQLITE_OK) {
        MessageBox(NULL, L"Failed to execute query", L"Error", MB_OK);
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_step(stmtCheck);
    if (rc != SQLITE_ROW) {
        MessageBox(NULL, L"Recipient account does not exist", L"Error", MB_OK);
        sqlite3_finalize(stmtCheck);
        sqlite3_close(db);
        return;
    }
    sqlite3_finalize(stmtCheck);

    if (currentUser.balance < dAmount) {
        MessageBox(NULL, L"Insufficient balance", L"Error", MB_OK);
        sqlite3_close(db);
        return;
    }

    sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, 0);

    std::stringstream ssDeduct;
    ssDeduct << "UPDATE users SET balance = balance - " << dAmount << " WHERE id = " << currentUser.userID << ";";
    std::string sqlDeduct = ssDeduct.str();
    rc = sqlite3_exec(db, sqlDeduct.c_str(), 0, 0, 0);
    if (rc != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", 0, 0, 0);
        MessageBox(NULL, L"Failed to update balance", L"Error", MB_OK);
        sqlite3_close(db);
        return;
    }

    std::stringstream ssAdd;
    ssAdd << "UPDATE users SET balance = balance + " << dAmount << " WHERE id = " << sRecipient << ";";
    std::string sqlAdd = ssAdd.str();
    rc = sqlite3_exec(db, sqlAdd.c_str(), 0, 0, 0);
    if (rc != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", 0, 0, 0);
        MessageBox(NULL, L"Failed to update recipient's balance", L"Error", MB_OK);
        sqlite3_close(db);
        return;
    }

    // Insert the transaction details into the transactions table
    std::stringstream ssInsertTransaction;
    ssInsertTransaction << "INSERT INTO transactions (from_user, to_user, amount) VALUES ("
        << currentUser.userID << ", " << sRecipient << ", " << dAmount << ");";
    std::string sqlInsertTransaction = ssInsertTransaction.str();
    rc = sqlite3_exec(db, sqlInsertTransaction.c_str(), 0, 0, 0);
    if (rc != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", 0, 0, 0);
        MessageBox(NULL, L"Failed to record transaction", L"Error", MB_OK);
        sqlite3_close(db);
        return;
    }

    sqlite3_exec(db, "COMMIT;", 0, 0, 0);

    currentUser.balance -= dAmount;

    sqlite3_close(db);

    MessageBox(NULL, L"Transfer successful", L"Success", MB_OK);

    CreateMainMenu(hwnd);
}


void HandleListAccounts(HWND hwnd) {
    ClearWindowControls(hwnd);

    sqlite3* db;
    sqlite3_open("bank.db", &db);

    std::stringstream ss;
    ss << "SELECT id, username FROM users;";
    std::string sql = ss.str();

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        MessageBox(NULL, L"Failed to execute query", L"Error", MB_OK);
        sqlite3_close(db);
        return;
    }

    HWND hDropdown = CreateWindow(L"combobox", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWN, 50, 50, 250, 100, hwnd, NULL, hInst, NULL);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int accountID = sqlite3_column_int(stmt, 0);
        const unsigned char* username = sqlite3_column_text(stmt, 1);

        std::wstring wUsername;
        for (int i = 0; username[i] != '\0'; ++i) {
            wUsername += (wchar_t)username[i];
        }

        std::wstringstream ws;
        ws << L"ID: " << accountID << L" - " << wUsername;
        SendMessage(hDropdown, CB_ADDSTRING, 0, (LPARAM)ws.str().c_str());
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    CreateWindow(L"button", L"Back", WS_VISIBLE | WS_CHILD, 50, 150, 80, 30, hwnd, (HMENU)ID_BACK, hInst, NULL);
    CreateWindow(L"button", L"Exit", WS_VISIBLE | WS_CHILD, 150, 150, 80, 30, hwnd, (HMENU)ID_EXIT, hInst, NULL);
}

void HandleNotice(HWND hwnd) {
    ClearWindowControls(hwnd);

    // Display Loan Rates
    CreateWindow(L"static", L"LOAN RATES", WS_VISIBLE | WS_CHILD, 50, 10, 200, 30, hwnd, NULL, hInst, NULL);
    CreateWindow(L"static", L"- HOME LOAN RATES: 6.5%", WS_VISIBLE | WS_CHILD, 50, 40, 200, 30, hwnd, NULL, hInst, NULL);
    CreateWindow(L"static", L"- PERSONAL LOAN RATES: 10.5%", WS_VISIBLE | WS_CHILD, 50, 70, 200, 30, hwnd, NULL, hInst, NULL);

    // Display Fixed Rates Update
    CreateWindow(L"static", L"FIXED RATES UPDATE", WS_VISIBLE | WS_CHILD, 50, 110, 200, 30, hwnd, NULL, hInst, NULL);
    CreateWindow(L"static", L"- 1 YEAR DEPOSIT: 5.75%", WS_VISIBLE | WS_CHILD, 50, 140, 200, 30, hwnd, NULL, hInst, NULL);
    CreateWindow(L"static", L"- 5 YEAR DEPOSIT: 7.25%", WS_VISIBLE | WS_CHILD, 50, 170, 200, 30, hwnd, NULL, hInst, NULL);

    CreateWindow(L"button", L"Logs", WS_VISIBLE | WS_CHILD, 50, 210, 80, 30, hwnd, (HMENU)ID_LOGS, hInst, NULL);
    CreateWindow(L"button", L"Back", WS_VISIBLE | WS_CHILD, 150, 210, 80, 30, hwnd, (HMENU)ID_BACK, hInst, NULL);
    CreateWindow(L"button", L"Exit", WS_VISIBLE | WS_CHILD, 250, 210, 80, 30, hwnd, (HMENU)ID_EXIT, hInst, NULL);
}

void HandleLogs(HWND hwnd) {
    ClearWindowControls(hwnd);

    sqlite3* db;
    sqlite3_open("bank.db", &db);

    std::stringstream ss;
    ss << "SELECT * FROM transactions WHERE from_user=" << currentUser.userID << " OR to_user=" << currentUser.userID << ";";
    std::string sql = ss.str();

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        MessageBox(NULL, L"Failed to execute query", L"Error", MB_OK);
        sqlite3_close(db);
        return;
    }

    int y = 10;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int fromUser = sqlite3_column_int(stmt, 1);
        int toUser = sqlite3_column_int(stmt, 2);
        double amount = sqlite3_column_double(stmt, 3);

        std::wstringstream ws;
        ws << L" | From: " << fromUser << L" | To: " << toUser << L" | Amount: $" << amount;
        CreateWindow(L"static", ws.str().c_str(), WS_VISIBLE | WS_CHILD, 10, y, 480, 20, hwnd, NULL, hInst, NULL);
        y += 30;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    CreateWindow(L"button", L"Back", WS_VISIBLE | WS_CHILD, 50, y, 80, 30, hwnd, (HMENU)ID_BACK, hInst, NULL);
    CreateWindow(L"button", L"Exit", WS_VISIBLE | WS_CHILD, 150, y, 80, 30, hwnd, (HMENU)ID_EXIT, hInst, NULL);
}



void CreateMainMenu(HWND hwnd) {
    ClearWindowControls(hwnd);

    CreateWindow(L"button", L"Check Balance", WS_VISIBLE | WS_CHILD, 50, 50, 150, 30, hwnd, (HMENU)ID_CHECK_BALANCE, hInst, NULL);
    CreateWindow(L"button", L"Transfer Money", WS_VISIBLE | WS_CHILD, 50, 100, 150, 30, hwnd, (HMENU)ID_TRANSFER, hInst, NULL);
    CreateWindow(L"button", L"Notice", WS_VISIBLE | WS_CHILD, 50, 150, 150, 30, hwnd, (HMENU)ID_NOTICE, hInst, NULL);

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
        CreateInitialScreen(hwnd);
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
            CreateTransferDialog(hwnd);
            break;
        case ID_TRANSFER_MONEY:
            HandleTransfer(hwnd);
            break;
        case ID_LIST_ACC:
            HandleListAccounts(hwnd);
            break;
        case ID_NOTICE:
            HandleNotice(hwnd);
            break;
        case ID_LOGS:
            HandleLogs(hwnd);
            break;
        case ID_MENU:
            CreateInitialScreen(hwnd);
            break;
        case ID_BACK:
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
