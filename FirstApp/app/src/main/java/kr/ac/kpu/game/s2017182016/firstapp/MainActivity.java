package kr.ac.kpu.game.s2017182016.firstapp;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private TextView mainTextView;
    private ImageView mainImageView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mainTextView = findViewById(R.id.mainTextView);
        mainTextView.setText("Program Started");
        mainImageView = findViewById(R.id.mainImageView);

//        Button helloButton = findViewById(R.id.helloButton);
//        helloButton.setOnClickListener(this);
//
//        Button worldButton = findViewById(R.id.worldButton);
//        helloButton.setOnClickListener(this);
    }

    public void onBtnHello(View view) {
            mainTextView.setText("2 / 5");
            mainImageView.setImageResource(R.mipmap.cat2);
    }

    public void onBtnWorld(View view) {
        mainTextView.setText("3 / 5");
        mainImageView.setImageResource(R.mipmap.cat3);
    }
}