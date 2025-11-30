#include "core/Repository.h"
#include "menu/Menu.h"
#include "utils/Utils.h"
#include "core/Config.h"

int main()
{
    // Màn hình loading trước
    showLoadingScreen();

    Repository repo;
    repo.loadAllData();

    Menu menu(repo);
    menu.run();

    return 0;
}
