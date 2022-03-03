import sys
from gurobi_optimization import optimization
full_text = ''
for i in range(0,31):
    instance_name = 'inst' + str(i)
    opt = optimization()
    opt.read_instance(name=instance_name)
    opt.run_optimization(time_limit=1800.0)
    line = 'instance '+str(i)+': Best Value: '+str(opt.optimal_value)+' Gap: '+str(opt.gap)+'%'+' Runtime: '+str(opt.runtime)+' s''\n'
    print(line)
    full_text += line
    print(instance_name+' has been saved in file')
    with open(r'/home/valdomiro/Programming/Algoritmos-TCC/linear programming/results/valdomiro_model_results.txt', 'w') as answer:
        answer.write(full_text)



    