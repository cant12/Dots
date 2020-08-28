#include<vector>
#include<iostream>
#include<algorithm>
#include<random>
#include<ctime>
using namespace std;

struct result
{
  vector<string> plan;
  float payoff;
};

int randomfunc(int j)
{
    return rand() % j;
}

class board
{
public:
  int w_homes,b_homes;
  vector<vector<int> > hor,ver,point;
  bool player,mute;
  int n;
  board(int len)
  {
    n = len;
    w_homes = 0;
    b_homes = 0;
    player = 1;
    mute = 0;
    for(int i=0;i<n;i++)
    {
      vector<int> temp;
      temp.empty();
      for(int j=0;j<n-1;j++)
        temp.push_back(0);
      hor.push_back(temp);
    }
    for(int i=0;i<n-1;i++)
    {
      vector<int> temp;
      temp.empty();
      for(int j=0;j<n;j++)
        temp.push_back(0);
      ver.push_back(temp);
    }
    for(int i=0;i<n-1;i++)
    {
      vector<int> temp;
      temp.empty();
      for(int j=0;j<n-1;j++)
        temp.push_back(0);
      point.push_back(temp);
    }
  }

  string chain_moves(bool debug)
  {
  	string ans = "";
  	board state = *this;
  	vector<string> moves = state.valid_moves(0,0);
  	int i,w = state.w_homes, b =state.b_homes;
  	if(debug)
  		cout << w << " " << b<<endl;
  	for(i=0;i<moves.size();i++)
  	{
  		if(state.make_move(moves[i]))
  		{
  			if(state.w_homes > w || state.b_homes > b)
  			{
  				ans += moves[i];
  				ans += ", ";
  				if(state.make_move("PASS"))
  					ans += state.chain_moves(debug);
  				break;
  			}
  		}
  		state = *this;
  	}
  	return ans;
  }

  vector<string> valid_moves(bool mode,bool chain)
  {
    vector<string> ans;
    string t = "";
    if(mute)
    {
      ans.push_back("PASS");
      return ans;
    }
    for(int i=0;i<n;i++)
    {
      for(int j=0;j<n-1;j++)
      {
        if(hor[i][j]==0)
        {
          t = "";
          t += i+48;
          t += " ";
          t += j+48;
          t += " ";
          t += i+48;
          t += " ";
          t += j+49;
          ans.push_back(t);
          if(mode)
          {
            t = "";
            t += i+48;
            t += " ";
            t += j+49;
            t += " ";
            t += i+48;
            t += " ";
            t += j+48;
            ans.push_back(t);
          }
        }
      }
    }
    for(int i=0;i<n-1;i++)
    {
      for(int j=0;j<n;j++)
      {
        if(ver[i][j]==0)
        {
          t = "";
          t += i+48;
          t += " ";
          t += j+48;
          t += " ";
          t += i+49;
          t += " ";
          t += j+48;
          ans.push_back(t);
          if(mode)
          {
            t = "";
            t += i+49;
            t += " ";
            t += j+48;
            t += " ";
            t += i+48;
            t += " ";
            t += j+48;
            ans.push_back(t);
          }
        }
      }
    }
    random_shuffle(ans.begin(),ans.end(),randomfunc);
    if(chain)
    {
    	string c = this->chain_moves(0);
    	if(c!="")
    		ans.insert(ans.begin(),"C "+c);
    }
    return ans;
  }

  bool make_move(string move)
  {
    int i;
    vector<string> moves = this->valid_moves(1,0);
    bool comp = 0;
    for(i=0;i<moves.size();i++)
    {
      if(moves[i]==move)
        break;
    }
    if(move[0]=='C')
    {
    	for(int start=2;start<move.length();start+=9)
    	{
    		this->make_move(move.substr(start,7));
    		this->make_move("PASS");
    	}
    	return true;
    }
    else if(i==moves.size())
      return false;
    //write
    if(move=="PASS")
    {
      mute = 0;
      player = !player;
      return true;
    }
    int x = min(move[0]-48,move[4]-48), y =  min(move[2]-48,move[6]-48);
    if(move[0]==move[4])
    {
      hor[x][y] = 1;
      if(x>0 && hor[x-1][y]==1 && ver[x-1][y]==1 && ver[x-1][y+1]==1)
      {
        if(player)
        {
          w_homes++;
          point[x-1][y] = 1;
        }
        else
        {
          b_homes++;
          point[x-1][y] = -1;
        }
        comp = 1;
      }
      if(x<n-1 && hor[x+1][y]==1 && ver[x][y]==1 && ver[x][y+1]==1)
      {
        if(player)
        {
          w_homes++;
          point[x][y] = 1;
        }
        else
        {
          b_homes++;
          point[x][y] = -1;
        }
        comp = 1;
      }
    }
    else
    {
      ver[x][y] = 1;
      if(y>0 && ver[x][y-1]==1 && hor[x][y-1]==1 && hor[x+1][y-1]==1)
      {
        if(player)
        {
          w_homes++;
          point[x][y-1] = 1;
        }
        else
        {
          b_homes++;
          point[x][y-1] = -1;
        }
        comp = 1;
      }
      if(y<n-1 && ver[x][y+1]==1 && hor[x][y]==1 && hor[x+1][y]==1)
      {
        if(player)
        {
          w_homes++;
          point[x][y] = 1;
        }
        else
        {
          b_homes++;
          point[x][y] = -1;
        }
        comp = 1;
      }
    }
    if(comp)
      mute = 1;
    player = !player;
    return true;
  }

  bool game_over()
  {
    return w_homes + b_homes == 16;
  }

  void print_board()
  {
    cout << "    0   1   2   3   4" << endl<<endl;
    for(int i=0;i<2*n-1;i++)
    {
      if(i%2==0)
      {
        cout << i/2 << "   ";
        for(int j=0;j<hor[0].size();j++)
        {
          if(hor[i/2][j]!=0)
            cout << "o---";
          else
            cout << "o   ";
        }
        cout << "o" << endl;
      }
      else
      {
        cout << "    ";
        // cout << "hi " << point.size() << " " << point.size()<<endl;
        for(int j=0;j<point[0].size();j++)
        {
          if(ver[i/2][j]!=0)
            cout << "|";
          else
            cout << " ";
          if(point[i/2][j]>0)
            cout << " w ";
          else if (point[i/2][j]<0)
            cout << " b ";
          else
            cout << "   ";
        }

        if(ver[i/2][n-1]!=0)
          cout << "|" << endl;
        else
          cout << " " << endl;
      }
    }
  }

  float utility()
  {
    return w_homes - b_homes;
  }

  result best_move(int look, float alpha, float beta, float t, bool f, vector<string> killer, bool kill)
  {
    clock_t starttime = clock();
    board state = *this;
    result ans, temp;
    int i,n,dc;
    string temps;
    if(look==0 || state.game_over())
    {
      ans.payoff = state.utility();
      return ans;
    }
    vector<string> actions = state.valid_moves(0,1),y;
    n = actions.size();
    if(player)
    {
      if(kill && killer.size()>0)
      {
        actions.insert(actions.begin(),killer[0]);
        killer.erase(killer.begin());
      }
      for(i=0,temp.payoff=alpha;i<n;i++)
      {
        if(f == 1 && ((float)(clock()-starttime)/CLOCKS_PER_SEC >= t))
        {
          y = temp.plan;
          y.insert(y.begin(),temps);
          ans.payoff = temp.payoff;
          ans.plan = y;
          return ans;
        }
        if((i==0 || actions[i]!=actions[0]) && state.make_move(actions[i]))
        {
          result x;
          if(actions[i]=="PASS")
            x = state.best_move(look,temp.payoff,beta,t-(float)(clock()-starttime)/CLOCKS_PER_SEC,0,killer,i==0 && kill);
          else
            x = state.best_move(look-1,temp.payoff,beta,t-(float)(clock()-starttime)/CLOCKS_PER_SEC,0,killer,i==0 && kill);
          if(temp.payoff < x.payoff)
          {
            temp = x;
            temps = actions[i];
            if(temp.payoff>=beta)
              break;
          }
        }
        state = *this;
      }
      y = temp.plan;
      y.insert(y.begin(),temps);
      ans.payoff = temp.payoff;
      ans.plan = y;
    }
    else
    {
      if(kill && killer.size()>0)
      {
        actions.insert(actions.begin(),killer[0]);
        killer.erase(killer.begin());
      }
      for(i=0,temp.payoff=beta;i<n;i++)
      {
        if(f == 1 && ((float)(clock()-starttime)/CLOCKS_PER_SEC >= t))
        {
          y = temp.plan;
          y.insert(y.begin(),temps);
          ans.payoff = temp.payoff;
          ans.plan = y;
          return ans;
        }
        if((i==0 || actions[i]!=actions[0]) && state.make_move(actions[i]))
        {
          result x;
          if(actions[i]=="PASS")
            x = state.best_move(look,alpha,temp.payoff,t-(float)(clock()-starttime)/CLOCKS_PER_SEC,0,killer,i==0 && kill);
          else
            x = state.best_move(look-1,alpha,temp.payoff,t-(float)(clock()-starttime)/CLOCKS_PER_SEC,0,killer,i==0 && kill);
          if(temp.payoff > x.payoff)
          {
            temp = x;
            temps = actions[i];
            if(temp.payoff<=alpha)
              break;
          }
        }
        state = *this;
      }
      y = temp.plan;
      y.insert(y.begin(),temps);
      ans.payoff = temp.payoff;
      ans.plan = y;
    }
    return ans;
  }

  result idd(float time_left)
  {
    clock_t starttime = clock();
    result ans, x;
    vector<string> killer;
    int i,j,n=0;
    for(i=0;i<hor.size();i++)
    {
      for(j=0;j<hor[i].size();j++)
      {
        if(hor[i][j]==0)
          n++;
      }
    }
    for(i=0;i<ver.size();i++)
    {
      for(j=0;j<ver[i].size();j++)
      {
        if(hor[i][j]==0)
          n++;
      }
    }
    float t = time_left/n,temp;
    if(player)
      ans.payoff = -16;
    else
      ans.payoff = 16;
    for(i=j,killer.clear();;i++)
    {
      temp = (float)(clock()-starttime)/CLOCKS_PER_SEC ;
      if(temp>=t)
      {
        // cout << i-1 << endl;
        break;
      }
      if(i==j)
      {
        x = best_move(i,-16,16,t-temp,0,killer,0);
        killer = x.plan;
      }
      else if(player)
        x = best_move(i,-16,16,t-temp,1,killer,1);
      else
        x = best_move(i,-16,16,t-temp,1,killer,1);
      if(player)
      {
        if(x.payoff > ans.payoff)
        {
          ans = x;
          killer = x.plan;
        }
      }
      else
      {
        if(x.payoff < ans.payoff)
        {
          ans = x;
          killer = x.plan;
        }
      }
    }
    return ans;
  }
};

int main()
{
  srand(unsigned(time(0)));
  int i;
  string move,start;
  bool choice;
  board* game = new board(5);
  vector<string> strategy;
  float time_elapsed1 = 0,time_elapsed2 = 0;

//  *****UI for playing against a player*****
  cout << "DOTS and HOUSES!" << endl<<endl;
  cout << "1. White starts first." << endl;
  cout << "2. A valid move is of the form \"v1 h1 v2 h2\" when u want to join dot1 to dot2 where v is the vertical coordinate and h is the horizontal coordinate."<<endl;
  cout << "3. Type \"yes\" when ready to begin." << endl;
  for(;;)
  {
    string s;
    getline(cin,s);
    if(s=="yes")
      break;
    else
      cout << "Try again." << endl;
  }
  cout << endl;
  cout << "Which colour do you chose?: ";
  getline(cin,start);
  cout << endl;
  if(start == "white")
    choice = 1;
  else
    choice = 0;
  game->print_board();
  cout << endl;
  // for(int i=0;i<temp.size();i++)
  //   cout << temp[i] << endl;
  // for(;;)
  // {
    // vector<string> temp = game->valid_moves();
  //   // cout << temp.size() << endl;
  //   if(game->player)
  //     cout << "White Player: ";
  //   else
  //     cout << "Black Player: ";
  //   getline(cin,move);
  //   if(game->make_move(move))
  //   {
  //     cout << endl;
  //     game->print_board();
  //     cout << endl;
  //   }
  //   else
  //     cout << "invalid move!" << endl;
  // }

  for(;;)
  {
    if(!game->mute)
    {
      if(game->player)
      {
        cout << "White Player: ";
        if(choice)
          getline(cin,move);
        else
        {
          clock_t starttime = clock();
          result x = game->idd(90-time_elapsed1);
          strategy = x.plan;
          if(strategy[0][0]=='C')
          	move = strategy[0].substr(2,7);
          else
          	move = strategy[0];
          cout << move << endl;// << endl;
          time_elapsed1 += (float)(clock()-starttime)/CLOCKS_PER_SEC;
          // cout << "Time left: " << 90-time_elapsed<<endl;
           // for(i=0;i<strategy.size();i++)
           //   cout << strategy[i] << endl;
           // cout << x.payoff << " " << game->utility() << endl;
        }

      }
      else
      {
        cout << "Black Player: ";
        if(!choice)
          getline(cin,move);
        else
        {
          clock_t starttime = clock();
          result x = game->idd(90-time_elapsed2);
          strategy = x.plan;
          if(strategy[0][0]=='C')
          	move = strategy[0].substr(2,7);
          else
          	move = strategy[0];
          cout << move << endl;// << endl;
          time_elapsed2 += (float)(clock()-starttime)/CLOCKS_PER_SEC;
          // cout << "Time left: " << 90-time_elapsed2<<endl;
           // for(i=0;i<strategy.size();i++)
           //    cout << strategy[i] << endl;
           // cout << x.payoff << " " << game->utility() << endl;
        }
      }
      cout << endl ;
      if(move=="end")
        break;
      if(game->make_move(move))
      {
        game->print_board();
        if(game->game_over())
        {
          cout << endl;
          if(game->utility()>0)
            cout << "The white player has won" << endl;
          else if(game->utility()<0)
            cout << "The black player has won" << endl;
          else
            cout << "Its a draw" << endl;
          break;
        }
      }
      else
        cout << "invalid move" << endl;
      cout << endl;
      //cout << game->whites.size()<<" "<< game->blacks.size() <<endl;
    }
    else
    {
      if(game->make_move("PASS")){}
    }
  }
  return 0;
}
