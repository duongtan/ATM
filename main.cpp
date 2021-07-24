#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

const int ERROR_USER_NOT_EXIST = -1;
const int ERROR_USER_WONG_PIN= -2;
const int ERROR_USER_BALANCE_NOT_ENOUGH = -3;
const int ERROR_USER_ACCID_NOT_EXIST = -4;

enum USER_ACTION
{
    ACT_VIEW_BALANCE = 0,
    ACT_WITHDRAW,
    ACT_DEPOSIT,
    ACT_UPDATE_PIN,
    ACT_EXIT
};

class User
{
private:
    int m_id;
    int m_pin;
    char m_name[64];
    char m_birthday[32];
    map<int,int> m_balance;
public:
    User() {}
    User(int id, int accNum,const char* name)
    {
        m_id = id;
        strcpy(m_name, name);
        if (accNum < 1) accNum = 1;
        for (size_t i = 0; i < accNum; i++)
        {
            m_balance[i] = 0;
        }
        
        m_pin = 123456;
    }
    ~User() {}

    vector<int> getAccIdList()
    {
        vector<int> ret;
        map<int, int>::iterator it = m_balance.begin();
        for (;  it!= m_balance.end(); it++)
        {
            ret.push_back(it->first);
        }
        return ret;
    }
    void setPIN(int pin) { m_pin = pin; }
    bool isCorrectPIN(int pin) { return m_pin == pin; }
    void setBirthDay(int year, int month, int day)
    {
        sprintf(m_birthday, "%d%02d%02d", year, month, day);
    }
    const char* getBirthDay() { return m_birthday; }

    bool isAccountIdExisted(int accId)
    {
        return m_balance.find(accId) != m_balance.end();
    }

    int getBalance(int accId) 
    { 
        return m_balance[accId]; 
    }
    void setBalance(int accId, int balance) {
        m_balance[accId] = balance;
    }
    void increaseBalance(int accId, int incr)
    { 
        m_balance[accId] += incr;
    }
    void decreaseBalance(int accId, int decr)
    { 
        m_balance[accId] -= decr;
    }
};

class Bank
{
private:
    int m_id;
    char m_name[64];
    map<int, User> m_activeUser;
    map<int, User> m_deleteUser;

    int getId()
    {
        int id = m_activeUser.size() + m_deleteUser.size() + 1;
        return id;
    }
public:
    Bank(int bankId, const char* bankName)
    {
        m_id = bankId;
        strcpy(m_name, bankName);
    }

    ~Bank() {}

    void createTestUser(int count, int numUserAccount)
    {
        char userName[64];
        for (size_t i = 0; i < count; i++)
        {
            sprintf(userName, "USER_%02d", i+1);
            int id = addUser(userName, numUserAccount);
            vector<int> accList = m_activeUser[id].getAccIdList();
            for (size_t j = 0; j < accList.size(); j++)
            {
                deposit(id, accList[j], j + (i + 1) * 5);
            }
        }
    }

    const char* getBankName() { return m_name; }

    bool isExistedUser(int userId)
    {
        if (m_activeUser.find(userId) == m_activeUser.end())
            return false;
        return true;
    }

    vector<int> getAccIdList(int userId)
    {
        vector<int> ret;
        if (m_activeUser.find(userId) != m_activeUser.end())
        {
            ret = m_activeUser[userId].getAccIdList();
        }
        
        return ret;
    }

    bool isExistedAccId(int userId, int accId)
    {
        if (m_activeUser.find(userId) == m_activeUser.end())
            return false;

        if (!m_activeUser[userId].isAccountIdExisted(accId))
            return false;

        return true;
    }

    int addUser(const char* userName, int accNum)
    {
        int id = getId();
        User acc(id, accNum, userName);
        m_activeUser[id] = acc;
        return id;
    }

    bool deleteUser(int id)
    {
        if (m_activeUser.find(id) == m_activeUser.end())
            return false;
        User acc = m_activeUser[id];
        m_activeUser.erase(id);
        m_deleteUser[id] = acc;
        return true;
    }

    const int checkPIN(int userId, int PIN)
    {
        if (m_activeUser.find(userId) == m_activeUser.end())
            return ERROR_USER_NOT_EXIST;

        if (!m_activeUser[userId].isCorrectPIN(PIN))
            return ERROR_USER_WONG_PIN;

        return 0;
    }

    const int checkBalance(int userId, int accId, int &balance)
    {
        if (m_activeUser.find(userId) == m_activeUser.end())
            return ERROR_USER_NOT_EXIST;
        if(!m_activeUser[userId].isAccountIdExisted(accId))
            return ERROR_USER_ACCID_NOT_EXIST;
        balance = m_activeUser[userId].getBalance(accId);
        return 0;
    }

    const int withdraw(int userId, int accId, int amount)
    {
        if (m_activeUser.find(userId) == m_activeUser.end())
            return ERROR_USER_NOT_EXIST;

        if (m_activeUser[userId].getBalance(accId) < amount)
            return ERROR_USER_BALANCE_NOT_ENOUGH;
        if(!m_activeUser[userId].isAccountIdExisted(accId))
            return ERROR_USER_ACCID_NOT_EXIST;
        m_activeUser[userId].decreaseBalance(accId, amount);

        return 0;
    }

    const int deposit(int userId, int accId, int amount)
    {
        if (m_activeUser.find(userId) == m_activeUser.end())
            return ERROR_USER_NOT_EXIST;

        if(!m_activeUser[userId].isAccountIdExisted(accId))
            return ERROR_USER_ACCID_NOT_EXIST;

        m_activeUser[userId].increaseBalance(accId, amount);
        return 0;
    }

    const int changePIN(int userId, int curPIN, int newPIN)
    {
        if (m_activeUser.find(userId) == m_activeUser.end())
            return ERROR_USER_NOT_EXIST;

        if (!m_activeUser[userId].isCorrectPIN(curPIN))
            return ERROR_USER_WONG_PIN;

        m_activeUser[userId].setPIN(newPIN);
    }
};

map<int, Bank*> bankList;

void initDataTest(int numUser, int numUserAccount)
{
    char name[64];
    for (size_t i = 0; i < 1; i++)
    {
        sprintf(name, "Bank_%02d", i + 1);
        Bank* bank = new Bank(i, name);
        bank->createTestUser(numUser, numUserAccount);
        bankList[i] = bank;
    }
}

void runATM()
{
    map<int, Bank*>::iterator itBank;
    int userId;
    int bankId = 0;
    int accId;
    int PIN;
    int actOP;
    int balance;
    int amount;
    while (1)
    {
        system("cls");
#if 0
        // step1: select bank
        printf("Please Select Bank:\n");

        for (itBank = bankList.begin(); itBank != bankList.end(); itBank++)
        {
            printf("\t %s: select %d\n", itBank->second->getBankName(), itBank->first);
        }
        cin >> bankId;

        if (bankList.find(bankId) == bankList.end())
        {
            printf("Bank Option Not Existed. Please try again\n");
            continue;
        }
        printf("(%s) is selected\n", bankList[bankId]->getBankName());
#endif
        
        // step2: insert card
        printf("Please Insert CARD\n");
        cin >> userId;

        if (!bankList[bankId]->isExistedUser(userId))
        {
            printf("CARD is INVALID. Please check again\n");
            continue;
        }
        // step3: insert PIN
        printf("Please Input PIN\n");
        cin >> PIN;
        if (ERROR_USER_WONG_PIN == bankList[bankId]->checkPIN(userId, PIN))
        {
            printf("PIN is wrong. Please check again\n");
            continue;
        }
ACCOUNT:
        // step4: Select Account PIN
        printf("Please Select Account:\n");
        vector<int> accList = bankList[bankId]->getAccIdList(userId);

        for (size_t i = 0; i < accList.size(); i++)
        {
            printf("\t Account %d : Select %d\n", accList[i],  i);
        }
        cin >> accId;
        accId = accList[accId];
        if (!bankList[bankId]->isExistedAccId(userId, accId))
        {
            printf("Selected Account is not in the list. Please Select again\n");
            goto ACCOUNT;
        }

    OPTION:
        // step4: select action
        printf("Account ID %d is selected.\n", accId);
        printf("Please Select Action:\n");
        for (size_t i = 0; i <= ACT_UPDATE_PIN; i++)
        {
            switch (i)
            {
            case ACT_VIEW_BALANCE:
                printf("\t%02d: View Balance\n", i);
                break;
            case ACT_WITHDRAW: 
                printf("\t%02d: Withdraw\n", i);
                break;
            case ACT_DEPOSIT:
                printf("\t%02d: Deposit\n", i);
                break;
            case  ACT_UPDATE_PIN:
                printf("\t%02d: Renew PIN\n", i);
                break;
            case ACT_EXIT:
                printf("\t%02d: Exit\n", i);
            }
        }

        cin >> actOP;
        if (actOP > ACT_UPDATE_PIN)
        {
            printf("Option Invalid. Please Select again\n");
            goto OPTION;
        }

        switch (actOP)
        {
        case ACT_VIEW_BALANCE:
            bankList[bankId]->checkBalance(userId, accId,balance);
            printf("Your Balance: %d\n", balance);
            break;
        case ACT_WITHDRAW:
            printf("Please Input Withdraw Amount:\n");
            cin >> amount;

            {
                int ret = bankList[bankId]->withdraw(userId, accId, amount);
                switch (ret)
                {
                case ERROR_USER_NOT_EXIST:
                    printf("Withdraw Failed. User Invalid\n");
                    continue;
                case ERROR_USER_BALANCE_NOT_ENOUGH:
                    printf("Withdraw Failed. Current balance not enough!");
                default:
                    printf("Withdraw successfully!\n");
                    bankList[bankId]->checkBalance(userId, accId,balance);
                    printf("Current Balance is : %d\n", balance);
                    break;
                }
            }
            break;
        case ACT_DEPOSIT:
            printf("Please Input Deposit Amount:\n");
            cin >> amount;

            {
                int ret = bankList[bankId]->deposit(userId, accId,amount);
                switch (ret)
                {
                case ERROR_USER_NOT_EXIST:
                    printf("Withdraw Failed. User Invalid\n");
                    continue;
                default:
                    printf("Deposit successfully!\n");
                    bankList[bankId]->checkBalance(userId, accId,balance);
                    printf("Current Balance is : %d\n", balance);
                    break;
                }
            }
            break;
        case  ACT_UPDATE_PIN:
        {
            int curPin, newPin;
            printf("Please Input Current PIN:\n");
            cin >> curPin;
            printf("Please Input New PIN:\n");
            cin >> newPin;
            int ret = bankList[bankId]->changePIN(userId, curPin, newPin);
            switch (ret)
            {
            case ERROR_USER_NOT_EXIST:
                printf("User Invalid\n");
                continue;
            case ERROR_USER_WONG_PIN:            
                printf("Current PIN is Wrong. Exit\n");
                continue;
                break;
            default:
                printf("New PIN is updated successfully\n");
                PIN = newPin;
                break;
            }
            break;
        }
        }

        int cont;
        printf("\nDo you want to do other action ?\n");
        printf("\tSelect 1 to continue\n");
        printf("\tSelect 0 to exit\n");
        cin >> cont;
        if (cont == 1)
            goto OPTION;            
    }
};

int main(void) {
    int numUser = 100;
    int numUserAccount = 3;
    initDataTest(numUser, numUserAccount);
    runATM();
    return 0;
}