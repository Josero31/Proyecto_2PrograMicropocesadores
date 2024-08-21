#include <iostream>
#include <fstream>
#include <sstream>
#include <mysql/mysql.h>
#include <omp.h>

void insertDataIntoDB(float latitud, float longitud, int altitud, const std::string& fecha, const std::string& hora, MYSQL* conn) {
    std::stringstream query;
    query << "INSERT INTO CoordenadasGPS (latitud, longitud, altitud, fecha, hora) VALUES ("
          << latitud << ", " << longitud << ", " << altitud << ", '" << fecha << "', '" << hora << "');";

    if (mysql_query(conn, query.str().c_str())) {
        std::cerr << "MySQL query error: " << mysql_error(conn) << std::endl;
    }
}

int main() {
    MYSQL* conn;

    conn = mysql_init(NULL);
    if (conn == NULL) {
        std::cerr << "mysql_init() failed\n";
        return EXIT_FAILURE;
    }

    if (mysql_real_connect(conn, "localhost", "root", "password", "TrackingDB", 0, NULL, 0) == NULL) {
        std::cerr << "mysql_real_connect() failed\n";
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    // User interaction to get the file path and other details
    std::string filePath;
    std::cout << "Ingrese la ruta del archivo .plt: ";
    std::cin >> filePath;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filePath << std::endl;
        return EXIT_FAILURE;
    }

    // Ignorar las primeras 6 líneas que no contienen datos relevantes
    std::string line;
    for (int i = 0; i < 6; ++i) {
        std::getline(file, line);
    }

    int numLines = 0;
    std::cout << "Ingrese la cantidad de líneas de datos que desea procesar (0 para todas las líneas): ";
    std::cin >> numLines;

    // OpenMP parallel section
    #pragma omp parallel
    {
        MYSQL* thread_conn = mysql_init(NULL);
        if (mysql_real_connect(thread_conn, "localhost", "root", "password", "TrackingDB", 0, NULL, 0) == NULL) {
            std::cerr << "mysql_real_connect() failed in thread\n";
            mysql_close(thread_conn);
            return EXIT_FAILURE;
        }

        int lineCount = 0;
        while (std::getline(file, line) && (numLines == 0 || lineCount < numLines)) {
            #pragma omp single nowait
            {
                std::stringstream ss(line);
                float latitud, longitud;
                int altitud;
                std::string fecha, hora;

                char comma; // Para manejar las comas en el archivo PLT
                ss >> latitud >> comma >> longitud >> comma >> comma >> altitud >> comma >> fecha >> comma >> hora;

                insertDataIntoDB(latitud, longitud, altitud, fecha, hora, thread_conn);
                ++lineCount;
            }
        }

        mysql_close(thread_conn);
    }

    mysql_close(conn);
    std::cout << "Datos insertados correctamente en la base de datos." << std::endl;
    return 0;
}