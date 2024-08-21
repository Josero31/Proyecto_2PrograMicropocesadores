#include <fstream>
#include <iostream>
#include <mysql/mysql.h>
#include <sstream>

void insertDataIntoDB(float latitud, float longitud, int altitud,
                      const std::string &fecha, const std::string &hora,
                      MYSQL *conn) {
  std::stringstream query;
  query << "INSERT INTO CoordenadasGPS (latitud, longitud, altitud, fecha, "
           "hora) VALUES ("
        << latitud << ", " << longitud << ", " << altitud << ", '" << fecha
        << "', '" << hora << "');";

  if (mysql_query(conn, query.str().c_str())) {
    std::cerr << "MySQL query error: " << mysql_error(conn) << std::endl;
  }
}

int main() {
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  conn = mysql_init(NULL);
  if (conn == NULL) {
    std::cerr << "mysql_init() failed\n";
    return EXIT_FAILURE;
  }

  if (mysql_real_connect(conn, "localhost", "root", "password", "TrackingDB", 0,
                         NULL, 0) == NULL) {
    std::cerr << "mysql_real_connect() failed\n";
    mysql_close(conn);
    return EXIT_FAILURE;
  }

  std::ifstream file("coordenadas.plt");
  std::string line;

  // Ignorar las primeras 6 líneas que no contienen datos relevantes
  for (int i = 0; i < 6; ++i) {
    std::getline(file, line);
  }

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    float latitud, longitud;
    int altitud;
    std::string fecha, hora;

    char comma; // Para manejar las comas en el archivo PLT
    ss >> latitud >> comma >> longitud >> comma >> comma >> altitud >> comma >>
        fecha >> comma >> hora;

    insertDataIntoDB(latitud, longitud, altitud, fecha, hora, conn);
  }

  mysql_close(conn);
  return 0;
}