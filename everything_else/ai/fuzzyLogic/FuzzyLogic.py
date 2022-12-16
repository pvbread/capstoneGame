import numpy as np
import skfuzzy as fuzz
import skfuzzy.membership as mf
import matplotlib.pyplot as plt

input_health = int(input("Health: "))
input_armor = int(input("Armor: "))
x_health = np.arange(0, 25, 1)
x_armor = np.arange(0, 10, 1)
y_threat = np.arange(0, 100, 1)

#4 stages of Health, [a, a, b, b] -> [b, b, c, c] -> [...]
health_nearDeath = mf.trapmf(x_health, [0, 1, 4, 5])
health_low = mf.trapmf(x_health, [4, 5, 9, 10])
health_med = mf.trapmf(x_health, [9, 10, 14, 15])
health_high = mf.trapmf(x_health, [14, 15, 24, 25])

#3 stages of Speed
armor_low = mf.trapmf(x_armor, [0, 1, 3, 4])
armor_med = mf.trapmf(x_armor, [3, 4, 6, 7])
armor_high = mf.trapmf(x_armor, [6, 7, 9, 10])

#3 stages Threat Level
threat_low = mf.trapmf(y_threat, [9, 18, 27, 35])
threat_med = mf.trapmf(y_threat, [27, 35, 51, 68])
threat_high = mf.trapmf(y_threat, [51, 68, 86, 100])

fig, (ax0, ax1, ax2) = plt.subplots(nrows = 3, figsize =(10,25))

#Plot health
ax0.plot(x_health, health_nearDeath, 'r', linewidth = 2, label = 'Near Death')
ax0.plot(x_health, health_low, 'g', linewidth = 2, label = 'Low Health')
ax0.plot(x_health, health_med, 'b', linewidth = 2, label = 'Med Health')
ax0.plot(x_health, health_high, 'y', linewidth = 2, label = 'High Health')
ax0.set_title('Health')
ax0.legend()

#Plot armor
ax1.plot(x_armor, armor_low, 'r', linewidth = 2, label = 'Low')
ax1.plot(x_armor, armor_med, 'g', linewidth = 2, label = 'Med')
ax1.plot(x_armor, armor_high, 'b', linewidth = 2, label = 'High')
ax1.set_title('Armor')
ax1.legend()

#Plot threat
ax2.plot(y_threat, threat_low, 'r', linewidth = 2, label = 'Low')
ax2.plot(y_threat, threat_med, 'g', linewidth = 2, label = 'Med')
ax2.plot(y_threat, threat_high, 'b', linewidth = 2, label = 'High')
ax2.set_title('Threat Level')
ax2.legend()

plt.tight_layout()

health_fit_nearDeath = fuzz.interp_membership(x_health, health_nearDeath, input_health)
health_fit_low = fuzz.interp_membership(x_health, health_low, input_health)
health_fit_med = fuzz.interp_membership(x_health, health_med, input_health)
health_fit_high = fuzz.interp_membership(x_health, health_high, input_health)

armor_fit_low = fuzz.interp_membership(x_armor, armor_low, input_armor)
armor_fit_med = fuzz.interp_membership(x_armor, armor_med, input_armor)
armor_fit_high = fuzz.interp_membership(x_armor, armor_high, input_armor)

#rules for the logic
rule1 = np.fmin(np.fmin(health_fit_nearDeath, armor_fit_low), threat_low)
rule2 = np.fmin(np.fmin(health_fit_nearDeath, armor_fit_med), threat_low)
rule3 = np.fmin(np.fmin(health_fit_nearDeath, armor_fit_high), threat_low)
rule4= np.fmin(np.fmin(health_fit_low, armor_fit_low), threat_low)
rule5= np.fmin(np.fmin(health_fit_low, armor_fit_med), threat_low)
rule6= np.fmin(np.fmin(health_fit_low, armor_fit_high), threat_low)
rule7 = np.fmin(np.fmin(health_fit_med, armor_fit_low), threat_med)
rule8 = np.fmin(np.fmin(health_fit_med, armor_fit_med), threat_med)
rule9 = np.fmin(np.fmin(health_fit_med, armor_fit_med), threat_med)
rule10 = np.fmin(np.fmin(health_fit_high, armor_fit_low), threat_high)
rule11 = np.fmin(np.fmin(health_fit_high, armor_fit_med), threat_high)
rule12 = np.fmin(np.fmin(health_fit_high, armor_fit_high), threat_high)

out_low = np.fmax(np.fmax(np.fmax(np.fmax(np.fmax(rule1, rule2), rule3), rule4), rule5), rule6)
out_med = np.fmax(np.fmax(rule7, rule8), rule9)
out_high = np.fmax(np.fmax(rule10, rule11), rule12)

threat0 = np.zeros_like(y_threat)

#honestly no idea what this is :thinking:
fig, ax0 = plt.subplots(figsize = (7,4))
ax0.fill_between(y_threat, threat0, out_low, facecolor = 'r', alpha = 0.7)
ax0.plot(y_threat, threat_low, 'r', linestyle = '--')
ax0.fill_between(y_threat, threat0, out_med, facecolor = 'g', alpha = 0.7)
ax0.plot(y_threat, threat_med, 'g', linestyle = '--')
ax0.fill_between(y_threat, threat0, out_high, facecolor = 'b', alpha = 0.7)
ax0.plot(y_threat, threat_high, 'b', linestyle = '--')
ax0.set_title('Threat Low')

#plt.tight_layout()
plt.show()

threat_level = np.fmax(np.fmax(out_low, out_med), out_high)

#defuzzified = fuzz.defuzz(y_threat, threat_level, 'centroid')
#result = fuzz.interp_membership(y_threat, threat_level, defuzzified)

#fig, ax0 = plt.subplots(figsisze = (7,4))

