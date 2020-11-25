#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

using ObjectId = unsigned long long int;

struct GameObject
{
    ObjectId id;
    string name;
    size_t x;
    size_t y;
};

#include "game_database.h"

template<class Stream>
void operator<<(Stream& s, const GameObject& obj)
{
    s << obj.id << ' ' << obj.name << ' ' << obj.x << ' ' << obj.y << '\n';
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    GameDatabase db;
    // *******************************
    size_t n;
    cin >> n;
    for (size_t i = 0; i < n; ++i)
    {
        size_t op_type, id, x, y;
        string name;
        cin >> op_type >> id;
        if (op_type) // remove operation
        {
            db.Remove(id);
            continue;
        }
        cin.ignore(1);
        cin >> name >> x >> y;
        db.Insert(id, std::move(name), x, y);
    }
    // *******************************
    for (size_t i = 0; i <= 49; ++i)
        for (size_t j = 0; j <= 49; ++j)
            for (const auto& e : db.DataByPosition(i, j))
                cout << e;
    for (char i = 'a'; i <= 'z'; ++i)
        for (char j = 'a'; j <= 'z'; ++j)
            for (const auto& e : db.DataByName(string() + i + j))
                cout << e;
    for (const auto& e : db.Data())
        cout << e;
    cout.flush();
    return 0;
}