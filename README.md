# HistoricalForexAnalysis

#### get Asia, EU, US market open time (Based on London time)
`cat usdx.csv | awk -F ',' '{print $1 " " $7}' | grep '02:00\|03:00\|10:00\|11:00\|14:00\|15:00' | sed -e 's/%//g' > analysis/data.txt`
