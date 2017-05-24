{

  vector<int> myvector;
  int myint;

  std::cout << "Please enter some integers (enter 0 to end):\n";

  do {
    std::cin >> myint;
    myvector.push_back (myint);
  } while (myint);

  cout << "myvector stores " << int(myvector.size()) << " numbers.\n";
  
  for (vector<int>::const_iterator i = myvector.begin(); i != myvector.end(); ++i)

	{
	cout << *i << endl;
	}

  return 0;
}
