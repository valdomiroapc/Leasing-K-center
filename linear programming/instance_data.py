
class instance:
    def __init__(self):
        self.T = None
        self.V = None
        self.L = None
        self.k = None
        self.d_i_j = None
        self.delta = None
        self.Dt = None
        self.instance_name = None
        self.instance_path = r'/home/valdomiro/Programming/Algoritmos-TCC/Instâncias-LKC'
    
    def read_instance(self, name):
        self.instance_name = name
        self.__read_instance()
    
    def __read_instance(self):
        instance_full_path = self.instance_path + '/' + self.instance_name + r'.txt'
        with open(instance_full_path, 'r') as instance_file:
            line = instance_file.readline()
            aux_list = line.split()
            #read V,T,L,k
            self.V = int(aux_list[0])
            self.T = int(aux_list[1])
            self.L = int(aux_list[2])
            self.k = int(aux_list[3])
            instance_file.readline()

            #read d(i,j)
            self.d_i_j = [[] for i in range(self.V)]
            for i in range(self.V):
                line = instance_file.readline()
                aux_list = line.split()
                for j in aux_list:
                    self.d_i_j[i].append(int(j))
            instance_file.readline()

            #read delta_l
            line = instance_file.readline()
            aux_list = line.split()
            self.delta = []
            for i in aux_list:
                self.delta.append(int(i))
            instance_file.readline()

            #read D_t
            self.Dt = [[] for i in range(self.T)]
            for i in range(self.T):
                line = instance_file.readline()
                aux_list = line.split()
                for j in aux_list[1:]:
                    self.Dt[i].append(int(j))


            
            

    
