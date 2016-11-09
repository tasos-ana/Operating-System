HY-345 / Exercise 3 / System Calls
Author: Anastasas Anastasios
Code: csd3166
Year: 2016-2017

Gia tin dimiourgia twn system call akolou8isa tin idia strtigikh/logikh me to
dummy system call.
Gia to set system call checkarete to pid an einai -1 | valide | wrong kai an einai
swsto kai epitrepto orizontai katallila ta paidia total_computation_time , remaining_time
kai infinite. An to pid einai >=0 tote elegxete h current diergasia kai ta paidia tis ean
kapoio apo afta einai to pid pou dothike

Gia to get system call elegxete arxika an to struct pou dinete einai NULL wste na epistrafei error
Ginontai oi idioi elegxoi gia to pid. An brethei to pid tote ginetai malloc to struct t_params
Stin sunexeia ginetai antigrafi dedomenon apo to struct tou pid sto t_params kai afto epistrefetai
ston user me tin copy_to_user ean dn uparxei kapoio problima.

Ta testfiles pou exw ftiaksei einai ena me pid=-1 to opoio einai to current kai h ektelesi tou
bgazei swsta apo telesmata
ena testfile me pid arnitiko kai diaforo tou -1 kai emfanizetai error
Telos alla duo testfile opou ginetai fork wste na ftiaxtei mia ne parent,child diergasia
opou sto 3o test file kathe diergasie elegxei to pid tis kai sto 4o, h parent elegxei tou child
enw tou child elegxei tou patera opou uparxei error se afto