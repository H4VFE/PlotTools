{

  vector<int> myvector;
  int myint;

  std::cout << "Please enter some integers (enter 0 to end):\n";

  do {
    std::cin >> myint;
    myvector.push_back (myint);
  } while (myint);

  cout << "myvector stores " << int(myvector.size()) << " numbers.\n";
  
  for (vector<int>::size_type i = 1; i != myvector.size(); i++) // i=1 means start at first, rather than zeroth element.

	{
	cout << "myvector [" << i << "] = " << myvector[i] << endl;
	}

  return 0;
}
