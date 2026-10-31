# 실제 영상과 CPU 포즈 연결: 느린 클립 시간과 주기적 큰 step

작성: 2026-10-31T10:18:35.885886+09:00

## 1. 작업 목표

느린 root 시계가 clip sample 입력까지 이어진 기존 결과를 실제 녹화가 있는 실행에서 재관측하고, CPU 본 포즈와 화면 표시가 서로 다른 시점에 갱신되는지 구분한다. 원인 확인 전 새 시간 주입·보간·업데이트 생략을 적용하지 않는다.64 기능·성능은 독립 실험으로 유지한다.

## 2. 실제 수행

기존 서명된 B8FB archive를 직접 사용하는 격리 launcher로 논리1개체/네이티브8파트·고정 run2·AI/루트 이동 OFF·queryPose ON·VSync ON·120초를 실행했다. 공용 D61D는 교체하지 않았다. 기존 entry-only Frida로 root/dispatcher/clip kernel을8초 관찰했고 인자/시간/결과를 변경하지 않았다.

기존 공식 portable OBS의 DayZ 창 전용 WGC·음성/바탕화면 OFF·1600×900·60fps·CRF0 설정을 재사용했다. 이번 출력 폴더만 지정했고 설정 변경 전 ini를 보존했다.45초 녹화 정상 완료 후 diagnostic recorder를 닫았으며, 게임의120초 정상 종료를 기다렸다. 실제 영상4개 시점의 contact sheet를 직접 확인해 actor ROI와 HUD 가시성을 검토했다.

기존 도구로 영상1/4/7/20/30초부터 각3초 구간을 분석했다. 다른 실행의 수동 확인 glyph anchor로 HUD F/T/A를 해독하고 이번 CPU TSV와 정확히 일치한 프레임만 사용했다. 임계값5/10/25/50pixel과0.1/1mm CPU RMS를 함께 보존했다. callback counter 기준±2frame 분류는 결과 확인 후 추가한 탐색 분석이며 사전 확정 검정이나 native 경계의 증명이 아니다.

영상 분석 기본 Python에 imageio_ffmpeg가 없어 첫 sheet 추출이 실행되지 않았다. 설치된 uv의 기존 캐시를 offline으로 사용해 imageio-ffmpeg0.7.0/Pillow 환경에서 성공했다. inherited analyzer의 videoRecorded=false 기본값은 정상 녹화 메타데이터로 수정했고 수정 전 파생본도 보존했다. 원시 기록을 수정한 것은 아니다.

## 3. 측정 결과

- 실행 runtime-20261031-101131, 정상 종료. root/dispatcher/kernel 각각 **103/103/103회**, observer 오류/drop0.
- clip 시간 증가 **5.100019455초 / host 7.883초 = 0.646964배**. root 증가량과 최대 잔차0. command+12는103회 모두 root dt와 일치.
- 이전 무녹화 실행의0.624845배와 이번0.646964배는 모두 느린 진행을 보였다. OBS 유무가 다르므로 엄격한 동일 조건 반복이나 개선 비교로 취급하지 않는다.
- 영상 **70926041bytes**, OBS output2701frames, rendering lag1frame, CRF0·60/1 확인. 실제 녹화이지 HUD 플래그만 있는 실행이 아니다.
- 선정 **900영상프레임 중883개 HUD 정확 일치**,17개 모호/불일치 제외. 인접714쌍 수용, 그중710쌍은 연속 mission frame. 중복 HUD150쌍에서10pixel 이상 몸 변화0.
- CPU RMS와 화면 윤곽 변화량 상관 **0.943549**(714쌍),연속710쌍 **0.942423**. lag±1/±2는 각각 음의 상관이었으며 전체 값을 보존했다.
-714쌍 모두5/10/25pixel 기준 몸 변화가 관측됐다.50pixel 기준1쌍만 작은 CPU 변화와 함께 기준 아래였다. HUD는 바뀌는데 몸만10pixel 기준으로 멈춘 구간은 이번5개 창에서0.
- 연속710쌍 중75mm 초과 CPU RMS step **158개**. callback counter 증가2frame 뒤 분류에는149개,나머지에는9개. 해당 두 집단 RMS 중앙값 **97.355/17.789mm**,화면 변화 중앙값 **3765.0/1068.0pixel**. 이 주기 때문에 미래2frame 분류에서도 유사 패턴이 생기므로 지연의 인과 확정은 금지한다.
-15~55초 전경 비율1.0,mission frame p50/p95/p99 **20.0/35.0/36.0ms**. OBS/훅 포함으로 최적화 수용에 사용하지 않는다.

## 4. 원인 분석

확인: CPU 본 변화와 실제 화면 윤곽 변화가 같은 HUD 프레임에서 강하게 연결됐다. 선택 고정 run의 이번 구간에서는 CPU만 움직이고 표시 포즈가 별도로 오래 멈추는 현상을 재현하지 못했다. clip 샘플 입력의 느린 진행은 영상이 있는 실행에서도 관측됐다.

관측된 큰 step은 화면에도 나타나며 callback 주기 주변의 특정 frame 위치에 집중된다. 따라서 정지 프레임 수만 세는 방법으로 사용자가 느낀 끊김을 모두 설명할 수 없다. 느린 전체 재생,프레임 간 시간 간격,주기적 포즈 증분의 불연속을 구분해야 한다.

가설: 평가·보간 세대 전환과 누적기 reset이 표시 step에 기여할 수 있다. 기존 정적/런타임 clock 증거와 양립하지만 이번 영상만으로 해당 명령의 인과를 확정하지 않는다. counter A는 스크립트 업데이트이며 실제 graph 평가 이벤트와 동일하지 않다. 영상 HUD와 CPU frame의 연결은 정확하지만 **native clip 호출 각각을 동일 영상 frame에 연결하지는 않았다**. GPU Present·관절 optical tracking·주관적 부드러움은 미측정이다.

## 5. 검증 수준

실제 녹화·HUD/CPU 포즈/화면 변화 연결: **진단 성공,1개체1회·선정5구간**. 느린 clip 시계: 서로 다른 OBS 조건의2실행에서 관측,엄격한 반복 수용 아님. callback lag: 탐색 가설. 수정 효과·전14clip·8/64 기능 회귀·64비용·20ms p95·시각 최종 수용: 미완료.

## 6. 변경 사항

빌드 B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185 유지. 새 PBO/재서명/배포 없음. 실행 전3개 의존 PBO의 기존 서명 검증 성공. 원시 기록·운영 서버·공용 D61D·개인키 보존. 기존 분석기 재사용과 격리 finish/join/counter 분류 및 출력만 추가했다. OBS 출력 위치 변경 전 설정을 보존했고 desktop/audio source는 추가하지 않았다.

## 7. 원시 증거

plan/attach-readiness/results/visible-join/callback-boundaries/callback-lag-exploration/pacing-driver와 profile의 launch/native-call-probe/visible-frame-clock/visible-frame-ids/visible-clip-analysis/visible-pose-alignment/obs-capture 기록 및 finalized MKV를 연결했다. 원시와 이전 보고서를 덮어쓰지 않았다. 각 입력 해시를 분석 때 대조하고 체크포인트 저장 전 재검증한다. 공개본은 선정 event/요약/분류와 함수만 포함하며 영상 전체·로컬 경로·실행 포인터·프로세스 정보·키를 제외한다.

## 8. 잔여 문제

다음은 **큰 포즈 step이 발생한 frame의 실제 native 보간 세대/누적기와 clip sample 시간을 함께 연결**하는 것이다. 이미 검증한 기존 native-clock/pose-frame 연결 도구를 우선 재사용하고,새 observer는 필요한 데이터만 제한적으로 추가한다. 정확한 native↔영상 frame 연결 없이±2frame 탐색 상관을 지연의 확정 원인으로 사용하지 않는다.

그 뒤 지원되는 시계/평가 제어가 존재하는지 계약을 확인하고 최소 후보1개를 별도 실행에서 비교한다. 현재 무작정 playback 배수·dt주입·프레임 생략은 적용하지 않는다.64 혼잡 원인 재현과 비용 귀속은 각각 별도 조건을 유지한다. 미지원 API/표시 단계가 확인되면 필요한 도구 또는 사용자 관찰을 명시한다.

선정 공개 본문. 로컬 보고서 SHA-256: `c58a69b2bce645db599506d382d8c4bf542436dac02cd33c13436ebf118c4a48`. 원시 전체가 아니다.
