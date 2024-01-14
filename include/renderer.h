extern struct notcurses* nc;

int initWindow();

void setTitle(const char* title);

struct ncplane* setArticleTitle();

struct ncplane* setSubTitle();

struct ncplane* initStdPlane();

void waitForStart();

int waitForKeys();

void showGameOver();

void initGameWindow();