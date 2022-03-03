import sys
from gurobi_optimization import optimization
instance_name = r'inst12'
opt = optimization()
opt.read_instance(name=instance_name)
opt.run_optimization(time_limit=1800.0)
print('valor ótimo encontrado: {valor_otimo}, gap: {gap}, runtime: {runtime}'.format(valor_otimo=opt.optimal_value,gap=opt.gap,runtime=opt.runtime))
