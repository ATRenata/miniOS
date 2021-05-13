int launcher(FILE *p);
int countTotalPages(FILE *f);
FILE *findPage(int pageNumber, FILE *f);
int findVictim(PCB *p);
int findFrame(FILE *page);
int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame);
int updateFrame(int frameNumber, int victimFrame, FILE *page);