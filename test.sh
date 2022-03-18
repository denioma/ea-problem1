for testcase in testcases/*.out; do
    filename=`echo $testcase | cut -d "." -f1`
    time ./main < $filename 1> /dev/null
    ./main < $filename | diff $filename.out -
    if [ $? -eq 0 ]; then
        echo "✅ PASSED $filename"
    else
        echo "❌ FAILED $filename"
    fi
done
