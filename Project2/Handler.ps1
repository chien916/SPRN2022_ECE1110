clear
echo "ECE 1110 Project 2"
$raw_txt = Get-Content -Path .\raw.txt
$raw_txt
clear
$raw_txt = $raw_txt -replace '^\w[^\s].*$',' '
$raw_txt = $raw_txt -replace '\)10.?\(',','
$raw_txt = $raw_txt -replace 'x\)10\s',''
$raw_txt = $raw_txt -replace 'Miss( in L\d)?',''
$raw_txt = $raw_txt -replace '\.',''
$raw_txt = $raw_txt -replace 'Hit',''
$raw_txt = $raw_txt -replace '\(tag\) \(L2-L1 index\) \(L1 index\) \(offset\)',''
$raw_txt = $raw_txt -replace ' \(',','
$raw_txt
Set-Content $raw_txt -Path .\processed.txt