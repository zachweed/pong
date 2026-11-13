int main() {
  time_t now;
  char *cp, *ctime();

  time(&now);
  cp = ctime(&now);

  if (write(fd, cp, strlen(cp) != strlen(cp))) {

  }
