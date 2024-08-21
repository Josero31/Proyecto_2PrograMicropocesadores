{ pkgs }:

{
  deps = [
    pkgs.gcc
    pkgs.mysql-connector-c
    pkgs.libmysqlclient
    pkgs.mysql
  ];
}
