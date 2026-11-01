# 머리 영상 추적의 오검출 확인·표시 지연 모델 미확정

작성: 2026-11-01T18:20:14.653404+09:00

## 1. 작업 목표

이미 종료·저장된 진단 영상에서 머리 표면 특징을 추적하고 자연 cache/scheduler 포즈와 시간적 연관성을 검사한다. 새 클라이언트·녹화·PBO·시간 보정 없이 기존 원시 자료를 활용한다. 정확한 camera projection이나 game frame identity가 없는 상태에서 지연을 단정하지 않는다.

## 2. 실제 수행

입력은 animation-diag-cache-visible-20261101-1750의 성공 runtime-2이며 녹화/프로세스 종료·설정 복원 및 기존 입력해시를 확인했다. 역사적 image-only 머리 cap detector 원리를 재사용하되 이번 카메라에 맞춘 색상 threshold120(주),110/130(민감도),radius13~20과 ROI를 별도로 기록했다. 기존 detector 파일과 이전 자료는 바꾸지 않았다.

첫 ROI90x100은 달리기 중 머리가 잘려 얼굴/목을 잡았다. 실패 결과를 보존하고 native 일치도를 보지 않은 채 영상 범위만 근거로 ROI180x190으로 확장했다. 색상/radius는 유지했다. v2 QC sheet의 과거2배 좌표스케일 잔존 오류도 발견해 원본 sheet를 보존하고1:1로 교정한 별도 sheet를 만들었다. 추적 관측 좌표는 변경하지 않았다.

대표6장에서는 주 추적점이 머리 상단에 놓였다. 독립적인 영상 품질 기준(threshold110/120 점수>=0.75, 거리<=3px)을 적용하고 시간 앞 절반을 학습·뒤 절반을 holdout으로 분리했다. native Head MS translation3성분 및 Head local quaternion4성분에 intercept를 더한 affine 연관 모델을 고정±200ms/5ms간격으로 탐색했다. camera calibration이나 engine 보정이 아닌 탐색적 모델이다.

마지막으로 holdout 오차가 가장 큰6장의 원본 영상을 직접 확인했다. 모두 두 색상 기준이 어깨/목을 함께 머리로 잘못 잡은 사례였다. 이6장을 사후 제외하거나 모델을 다시 맞추지 않았다.

## 3. 측정 결과

기존 영상/native 겹침931프레임 중 영상 기준 통과861·제외70프레임이다. lag/비교대조 공통 범위는776개, 학습398·holdout378개다. threshold110 대주기준120 위치차 p50/p95/p99=0/1.414/45.539px이며 threshold130은1/45.497/49.336px다. 꼬리 오차가 크므로 전체 프레임의 동일 머리 추적을 인증할 수 없다.

학습 오차만으로 선택된 시프트는+50ms였다. 선택모델 학습RMS5.394px, holdout RMS7.938px·중앙값2.556px·p958.868px다. 0ms모델 holdout RMS12.967px이며 ±500/700ms 대조모델은18.837~20.804px다. 단위는 픽셀이며 frame-time이 아니다. 선택50ms는 실측 render/display latency가 아니다.

최대 holdout 오차6건은41.973~42.969px이며 image frame1044/1045/1170/1171/1295/1296이다. 직접 확인한 주 추적점은 머리가 아닌 어깨/목에 있었다. 두 색상 기준의 일치만으로 오검출이 제거되지 않았다. 따라서 이 모델로 cache-to-display 지연을 확정하지 않는다.

새8/64 기능 회귀·프레임 시간 p50/p95/p99·GPU·할당/GC·14동작 검증은 미실행이다. 기능·시각 품질·성능 개선 및20ms p95 달성을 주장하지 않는다. 새 runtime0·영상0·PBO0이며 기존 영상 추가복사 없이 작은 ROI 파생 관측/검토 그림만 만들었다.

## 4. 원인 분석

확인된 사실: 초기ROI는 실제 애니메이션 범위를 포함하지 못했다. 확대 후에도 밝은 무텍스처 모델의 머리/어깨가 비슷한 색과 곡률을 가져 detector가 부위를 혼동한다. 두 threshold가 함께 오검출할 수 있으므로 일치 여부는 충분조건이 아니다. 대표6장 점검만으로931프레임을 인증할 수도 없다.

추정: native와 영상 움직임 사이에 시간적 연관성이 있을 가능성은 모델오차와 대조결과에서 보인다. 그러나 empirical camera/표면 mapping오차, 비동기 WGC texture 나이, 혼합된cache/scheduler 관측단계, phase별 표본제외 및 detector 오검출이 섞여 있다.50ms를 엔진 지연이나 끊김 원인으로 정하지 않는다.

이번에 배제한 해석은 '두 색상 기준이 일치하면 같은 본을 확실히 추적한다'와 '최저 오차 lag가 곧 렌더 지연이다'이다. 기존 mode0 누적기 손실·변환계층·64개체 비용 근거를 반증하거나 대체하지 않는다.

## 5. 검증 수준

초기ROI와 image-only 추적의 실패 원인 구분은 진단 성공이다. native/화면 정확한 pose identity·렌더 지연·시각적 부드러움 판정은 미확정이다. 탐색적 회귀 fitting은 완료됐으나 성공적인 표시 지연 측정이나 회귀 통과·반복 검증·최종 수용으로 분류하지 않는다.

## 6. 변경 사항

별도 소규모 분석계획·ROI 추적·affine 연관성 분석·worst-case 검토 스크립트/JSON/PNG를 추가했다. 입력 영상·native log·게임코드·PBO·운영서버·배포본·기존 detector는 변경하지 않았다. 새 PBO가 없으므로 신규 서명 대상은 없다. 이번에는 client를 실행하지 않았으며 이전 성공실행의 기존3개 PBO 서명 검증을 새 실행의 검증처럼 세지 않는다.

## 7. 원시 증거

분석ID animation-diag-optical-association-20261101-1810. plan/plan-v2, head-observations/v2, landmark-qc-v1, corrected-coordinate QC, association-protocol/model/results, lag-scan, holdout-worst-six 및 검토 그림을 보존한다. 최초 v2 overlay 오류 그림도 비교 이력으로 보존한다. 입력 영상은 기존534735132bytes·SHA9354604a9746687934ce25f431784f70a9b3c240d0da7c7f2071dbd6257cc592 하나를 재사용했다. 공개본은 집계·대표오차·실패기준·한계를 제공하며 원시 영상/로컬경로/PID/CLI/키/게임자산은 제외한다.

해시·JSON·안전종료 검증 뒤 기존 MD/TXT 체크포인트와 B안 공개 자료를 갱신한다. 이번 새 분석시각과 기존 runtime의 촬영시각을 구분하며 새 게임 실행이 있었던 것처럼 갱신하지 않는다.

## 8. 잔여 문제

다음 우선순위는 threshold를 native결과에 맞춰 반복조절하는 일이 아니라 기존 HUD 프레임 라벨/clock 계측을 network 진단 client에도 연결하는 것이다. frame ID·appActive·개체식별·카메라/좌표투영의 실제 기록 가능 API와 실행 위치를 먼저 확인한다. 기존 visible-frame probe는 mission.c 기반 단독 fixture 경로이므로 network client에서 그대로 작동했다고 가정하지 않는다.

가능하면 본 getter를 추가 호출하지 않고 이미 갱신된 native matrix와 개체 transform·카메라/고정기준점 projection을 연결한다. 본 getter를 호출하면 lazy cache 갱신 시점을 바꿀 수 있으므로 그 경우는 별도 관측 조건이며 이전 자연cache 실행과 같은 baseline으로 쓰지 않는다. HUD/기록 오버헤드는 성능 비교에서 제외한다.

성공 분기: 실제 영상의 client frame label과 native 관측 순서를 연결한 뒤 pose/표면 projection차를 분석한다. 실패 분기: frame identity·clock 원점·대상개체 중 하나라도 불명확하면 정확한 latency판정을 보류한다. 원인 전 clock scale·보간·프레임 생략·collision 축소·64개체 redesign은 적용하지 않는다.

선정 공개 본문. 로컬 보고서 SHA-256: `d5b9d6290a78cb2bb5d2cadab0d046104050e6ba833bf103e63b7c615e8232f4`. 원시 전체가 아니다.
