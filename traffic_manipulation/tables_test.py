import pandas as pd

# assign data
a = {
    "Name": ["Nikhil", "Ravi", "Manish", "Prince"],
    "City": ["Delhi", "Kanpur", "Ahmedabad", "Bangalore"]
}

# create DataFrame
df = pd.DataFrame(a)

print(df)
