#include "run.h"
#include <getopt.h>
#include <errno.h>

void writeTimeMesssage(FILE* logfile, char* msg){
    time_t t = time(NULL);
    struct tm* aTm = localtime(&t);
    fprintf(logfile, "%04d/%02d/%02d %02d:%02d:%02d : %s\n", aTm->tm_year + 1900, aTm->tm_mon + 1,
            aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec, msg);
}

static void runRMAST(Tree* intree1,
    Tree* intree2,
    FILE* logfile,
    char* mast_treepath,
    char distance){

    writeTimeMesssage(logfile, "RMAST execution started");

    Branch* maxbranch;
    int maxL;
    Tree* tree1;
    Tree* tree2;
    Tree* mast_tree = NULL;

    treesToCommonLeaves(intree1, intree2, &tree1, &tree2);
    if (!tree1){
        fprintf(logfile, "Trees are completely different\n");
        return;
    }

    if (intree1->leavesNum != tree1->leavesNum ||
        intree2->leavesNum != tree2->leavesNum){
        writeTimeMesssage(logfile, "Trees doesn't had the same leaves and have been pruned");
        if (intree1->leavesNum != tree1->leavesNum){
            char* tempStr = treeToString(tree1);
            fprintf(logfile, "The first tree was changed: %s\n", tempStr);
            free(tempStr);
        }
        if (intree2->leavesNum != tree2->leavesNum){
            char* tempStr = treeToString(tree2);
            fprintf(logfile, "The second tree was changed: %s\n", tempStr);
            free(tempStr);
        }
    }

    writeTimeMesssage(logfile, "Searching for the maximum agreement subtree");
    int bestL  = RMAST(tree1, tree2, &mast_tree);
    writeTimeMesssage(logfile, "Done");
    char* tempStr = treeToString(mast_tree);
    fprintf(logfile, "The maximum agreement subtree: %s\n", tempStr);

    FILE* out = fopen(mast_treepath, "w");
    if (out == NULL){
        fprintf(stderr, "Error: cannot open outfile.\n");
        exit(EACCES);
    } 
    fputs(tempStr, out);
    fclose(out);
    free(tempStr);

    fprintf(logfile, "The size of subtree: %d\n", mast_tree->leavesNum);
    fprintf(logfile, "The corresponding L-distance: %d\n", bestL);
    
    if (distance){
        fprintf(stdout, "%d\n", bestL);
    }else{
        fprintf(stdout, "%d\n", mast_tree->leavesNum);
    }

    treeDelete(mast_tree);
    treeDelete(tree1);
    treeDelete(tree2);
} /* runRMAST */


static void runUMAST(Tree* intree1,
    Tree* intree2,
    FILE* logfile,
    char* mast_treepath,
    char distance) {
    writeTimeMesssage(logfile, "UMAST execution started");

    Tree* mast_tree = NULL;
    Tree* tree1;
    Tree* tree2;
    treesToCommonLeaves(intree1, intree2, &tree1, &tree2);
    if (!tree1){
        fprintf(logfile, "Trees are completely different\n");
        return;
    }

    if (intree1->leavesNum != tree1->leavesNum ||
        intree2->leavesNum != tree2->leavesNum){
        writeTimeMesssage(logfile, "Trees doesn't had the same leaves and have been pruned");
        if (intree1->leavesNum != tree1->leavesNum){
            char* tempStr = treeToString(tree1);
            fprintf(logfile, "The first tree was changed: %s\n", tempStr);
            free(tempStr);
        }
        if (intree2->leavesNum != tree2->leavesNum){
            char* tempStr = treeToString(tree2);
            fprintf(logfile, "The second tree was changed: %s\n", tempStr);
            free(tempStr);
        }
    }


    writeTimeMesssage(logfile, "Searching for the maximum agreement subtree");
    int bestL  = UMAST(tree1, tree2, &mast_tree);
    writeTimeMesssage(logfile, "Done");
    char* tempStr = treeToString(mast_tree);
    fprintf(logfile, "The maximum agreement subtree: %s\n", tempStr);

    FILE* out = fopen(mast_treepath, "w");
    if (out == NULL){
        fprintf(stderr, "Error: cannot open outfile.\n");
        exit(EACCES);
    } 
    fputs(tempStr, out);
    fclose(out);
    free(tempStr);

    fprintf(logfile, "The size of subtree: %d\n", mast_tree->leavesNum);
    fprintf(logfile, "The corresponding L-distance: %d\n", bestL);
    
    if (distance){
        fprintf(stdout, "%d\n", bestL);
    }else{
        fprintf(stdout, "%d\n", mast_tree->leavesNum);
    }

    treeDelete(mast_tree);
    treeDelete(tree1);
    treeDelete(tree2);
} //runUMAST

void showShortHelp(char *program_name){
    printf("Usage: %s -1 tree1 -2 tree2 [-r] [-o outtree] [-l logfile] [-d]\n", program_name);
    exit(0);
}

void showLongHelp(char *program_name){
    printf("Usage: %s -1/--tree1 tree1_path -2/--tree2 tree2_path\n", program_name);
    puts("[-r/--rooted rooted] [-o/--outfile outtreefile] [-l/--logfile logfile] [-d/--distance distance]");
    puts("-1/--tree1 path to the first tree file in Newick format");
    puts("-2/--tree2 path to the second tree file in Newick format");
    puts("-r/--rooted are trees rooted (search for the unrooted or the rooted MAST)");
    puts("-o/--outfile path to file for writing found MAST in Newick format (default: umast.tre)");
    puts("-l/--logfile path to file for writing logs (default: umast.log)");
    puts("-d/--distance print L-distance between input trees instead of UMAST size to stdout");
    exit(0);
}

typedef struct{
    char* tree1_path;
    char* tree2_path;
    char rooted;
    char* out_path;
    char* log_path;
    char distance;
} Args;

Args* argsParse(int argc, char** argv){
    Args* args = malloc(sizeof(Args));
    args->tree1_path = NULL;
    args->tree2_path = NULL;
    args->rooted = 0;
    args->out_path = "umast.tre";
    args->log_path = "umast.log";
    args->distance = 0;

    static struct option long_options[] = {
        {"tree1", required_argument, NULL, '1'},
        {"tree2", required_argument, NULL, '2'},
        {"rooted", no_argument, NULL, 'r'},
        {"outfile", required_argument, NULL, 'o'},
        {"logfile", required_argument, NULL, 'l'},
        {"help", no_argument, NULL, 'h'},
        {"distance", no_argument, NULL, 'd'},
        {NULL, 0, NULL, 0}
    };
    int option_index = -1;
    int opt = getopt_long(argc,
        argv,
        "1:2:ro:l:hd",
        long_options,
        &option_index);

    while ( opt != -1 ){
        switch (opt){
            case '1':
                args->tree1_path  = optarg;
                break;
            case '2':
                args->tree2_path = optarg;
                break;
            case 'r':
                args->rooted = 1;
                break;
            case 'o':
                args->out_path = optarg;
                break;
            case 'l':
                args->log_path = optarg;
                break;
            case 'd':
                args->distance = 1;
                break;
            case 'h':
                fprintf(stdout, "%d\n", option_index); //!!!!!!!
                if (option_index > 0){ // --help
                    showLongHelp(argv[0]);
                }else{ // -h
                    showShortHelp(argv[0]);
                }
                break;
            case '?':
                break;
            default:
                break;
        }
        opt = getopt_long(argc,
            argv,
            "1:2:ro:l:hd",
            long_options,
            &option_index);
        option_index = - 1;
    }

    if (!args->tree1_path){
        fprintf(stderr, "1st tree path must be specified\n");
        exit(EINVAL);
    }
    if (!args->tree2_path){
        fprintf(stderr, "2st tree path must be specified\n");
        exit(EINVAL);
    }

    return args;
}

int main(int argc, char** argv) {
    Tree* tree1;
    Tree* tree2;
    FILE* logfile;
    char* temp_string;

    Args* args = argsParse(argc, argv);

    if(strcmp(args->log_path, "umast.log") == 0){
        logfile = fopen("umast.log", "w");
    }else{
        logfile = fopen(args->log_path, "w");
    }
    if (logfile==NULL){
        fprintf(stderr, "Error: cannot open log file.\n");
        exit(EACCES);
    }

    if (args->rooted){
        tree1 = treeRead(args->tree1_path, 1);
        tree2 = treeRead(args->tree2_path, 1);
    }else{
        tree1 = treeRead(args->tree1_path, 0);
        tree2 = treeRead(args->tree2_path, 0);
    }

    fprintf(logfile, "Trees are read successfully\n");
    temp_string = treeToString(tree1);
    fprintf(logfile, "%s\n", temp_string);
    free(temp_string);
    temp_string = treeToString(tree2);
    fprintf(logfile, "%s\n", temp_string);
    free(temp_string);

    if (args->rooted){
        runRMAST(tree1,
                 tree2,
                 logfile,
                 args->out_path,
                 args->distance);
    }else{
        runUMAST(tree1,
                 tree2,
                 logfile,
                 args->out_path,
                 args->distance);
    }
    treeDelete(tree1);
    treeDelete(tree2);
    fclose(logfile);
    return 0;
}
