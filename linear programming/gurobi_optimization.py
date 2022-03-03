from instance_data import instance
from gurobipy import *
class optimization:
    def __init__(self):
        self.data = instance()
        self.optimizer = Model('LKC')
        self.x = None
        self.y = None
        self.z = None
        self.optimal_value = None
        self.gap = None
        self.runtime = None

    def read_instance(self,name = 'inst1'):
        self.data.read_instance(name)
    
    def define_variables(self):
        self.x = self.optimizer.addVars(self.data.V,self.data.V,self.data.T, vtype=GRB.BINARY, name='j is served by i')
        self.y = self.optimizer.addVars(self.data.V,self.data.L,self.data.T, vtype=GRB.BINARY, name='i with type l,start to serve at t')
        self.z = self.optimizer.addVar(lb=0.0,vtype=GRB.CONTINUOUS,name='objective variable')
    
    def define_objective(self):
        self.optimizer.setObjective(self.z,GRB.MINIMIZE)
    
    def define_constraint1(self):
        for t in range(self.data.T):
            for i in range(self.data.V):
                for j in self.data.Dt[t]:
                    self.optimizer.addLConstr(self.x[i,j,t]*self.data.d_i_j[i][j] <= self.z)

    def define_constraint2(self):
        for t in range(self.data.T):
            for j in self.data.Dt[t]:
                exp = LinExpr()
                for i in range(self.data.V):
                    exp += self.x[i,j,t]
                self.optimizer.addLConstr(exp == 1)

    def define_constraint3(self):
        for t in range(self.data.T):
            exp = LinExpr()
            for i in range(self.data.V):
                for l in range(self.data.L):
                    ptl = max(t - self.data.delta[l] +1, 0)
                    for t_ in range(ptl,t+1):
                        exp += self.y[i,l,t_]
            self.optimizer.addLConstr(exp <= self.data.k)

    def define_constraint4(self):
        for t in range(self.data.T):
            for i in range(self.data.V):
                for j in self.data.Dt[t]:
                    exp = LinExpr()
                    for l in range(self.data.L):
                        ptl = max(t - self.data.delta[l] +1, 0)
                        for t_ in range(ptl,t+1):
                            exp+=self.y[i,l,t_]    
                    self.optimizer.addLConstr(exp >= self.x[i,j,t])
    
    def define_constraint5(self):
        for t in range(self.data.T):
            for i in range(self.data.V):
                exp = LinExpr()
                for l in range(self.data.L):
                    ptl = max(t - self.data.delta[l] +1, 0)
                    for t_ in range(ptl,t+1):
                        exp += self.y[i,l,t_]
                self.optimizer.addLConstr(exp <= 1)

    def execute(self,time_limit):
        self.optimizer.Params.timeLimit = time_limit
        self.optimizer.optimize()

    def run_optimization(self,time_limit = 3600.0):
        self.define_variables()
        self.define_objective()
        self.define_constraint1()
        self.define_constraint2()
        self.define_constraint3()
        self.define_constraint4()
        self.define_constraint5()
        self.execute(time_limit)
        self.optimal_value = self.optimizer.ObjVal
        self.gap = self.optimizer.MIPGap*100.0
        self.runtime = self.optimizer.Runtime
    



