# Meteo

Meteo is a program to create multiples meteo graphics from huge file data .csv

## Deploy on Linux

```script shell
  make re
```

## Exemple 

create a wind graph only in France

```script shell
./meteo.sh -w -F -f meteo_filtered_data_v1.csv
```

## Args list


### Graph type [ Need one or more ]

#### -m

Moisture graph

Exemple:

```script shell
./meteo.sh -m -f meteo_filtered_data_v1.csv
```
<img src="https://i.imgur.com/ZawLMKM.png">

#### -h

Humidity graph

Exemple:

```script shell
./meteo.sh -h -f meteo_filtered_data_v1.csv
```

<img src="https://i.imgur.com/Vuckrxr.png">

#### -w

Wind graph

Exemple:

```script shell
./meteo.sh -w -f meteo_filtered_data_v1.csv
```

<img src="https://i.imgur.com/vM2ppZo.png">

### Location [ One or nothing ]

#### -F

France

Exemple:

```script shell
./meteo.sh -w -F -f meteo_filtered_data_v1.csv
```

<img src="https://i.imgur.com/VftIaLS.png">

### Sort mode [ One or nothing ( --avl by default )]

#### --tab

Linked List 

#### --abr

Binary Search Tree

#### --avl

Adelson-Velsky and Landis
self-balancing Binary Search Tree