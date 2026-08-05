#include <iostream>
#include <string>

std::string get_simple_debian_control_file()
{
    return
        "DAHER DAHER DAHER DAHER $$$$$$$$$$$$$$$$$$$$$$$$$$\n Source: hello-example\n"
        "Section: utils\n"
        "Priority: optional\n"
        "Maintainer: Your Name <you@example.com>\n"
        "Standards-Version: 4.6.2\n"
        "Build-Depends: debhelper-compat (= 13)\n"
        "\n"
        "Package: hello-example\n"
        "Architecture: any\n"
        "Depends: ${shlibs:Depends}, ${misc:Depends}\n"
        "Description: simple Debian package example\n"
        " A tiny example package used to show the format of a\n"
        " Debian debian/control file.\n";
}

int main()
{
    std::cout << "Simple Debian debian/control example:\n\n";
    std::cout << get_simple_debian_control_file();
    return 0;
}
