# 충돌 외64개체 비용 후보 검토 — 2026-11-06

## 1. 작업 목표
기존64개체 로그에서 충돌 외 비용 후보와 실제 수정 가능 지점을 찾는다. 호출 횟수와CPU시간을 구분하며 추가게임/계측을 하지 않는다.
## 2. 실제 수행
ANIM-playable-sixtyfour1의계획·목표·콜백카운터,server-frames의두원시TickCount구간,로그크기,사망시점과프레임을분석했다. 미션Commit과DGBGroundAgent·Ground·Combat·Traversal·Pursuit소스,기존5파트비용범위보고서및루트조회기각보고서를대조했다.
## 3. 측정 결과
| 후보 | 기존자료에서 확인한 규모 | 판정 한계 |
|---|---|---|
|경로 계획·목표 갱신|계획진입19,304·실패4,393·GOAL_APPLY17,192.개체ID/목표revision쌍도17,192|목표적용로그의동일revision반복은아님.경로진입은순수FindPath호출횟수와다르며SampleNavmesh/Detour등포함가능|
|초기전원생존비용표본주변|게임34.78~65초의계획진입3,124·실패228·목표적용2,802|다른후반결함없이도활동량이많음.원시CPU self-time미측정|
|320파트표시동기화|미션9,581프레임×320파트×GetPosition/GetOrientation2회=최소6,131,840회로소스기반산출|실제getter프로파일호출계수아님.조건부SetPosition/Orientation횟수는미기록.사망표시파트도루프에남음|
|사망후피해수신기설정반복|로그사망시점+미션프레임으로Commit의FreezeDamageReceiver진입약100,410회추정|지점별실측카운터아님.별도Observe호출도존재.사망후구간후보이며시체전초기27.66ms를설명하지않음|
|진단파일출력|서버TSV/log/RPT약33.26MB/305초.목표적용17,192행각각Open/Write/Close호출|33MB라는크기만으로병목판정불가.직렬화·파일열기닫기·디스크대기·로그OFF비교미측정|
|네이티브콜백/호스트|리더AnimUpdates·PhysicsUpdates각390,080.표시320파트와피해64수신기유지|리더카운터는전체파트포즈평가수아님.네이티브애니메이션평가/렌더self-time미측정|

server-frames의**두계측구간합계안에서만**Update측약85.3%,Commit측약14.7%의누적rawTickCount가관측된다.이는Director.Update+Crowd.Update+FrameFall와표시Commit이라는포함경과시간범위다.시간단위보정/스케줄링영향분리없음.전체CPU비중·게임전체프레임비중이아니며네이티브콜백과로그관측등측정밖작업이있다.14.7%전체CPU절감가능이라는해석은금지한다.
## 4. 원인 분석
확인된낭비후보: FreezeDamageReceiver는DamageReceiverDetached를true로쓰지만진입시이미설정됐는지검사하지않고충돌레이어·각geom레이어·중력OFF·INACTIVE를반복설정한다. 위치동기화는사망체이동때필요하므로함수전체를조기반환하라는결론은아니다. 반복물리설정과필요한위치동기화를분리할국소후보다. 물리body재생성시무효화도고려해야한다.
확인된대량작업: 경로진입/목표revision갱신과320파트매프레임transform조회. 이동중목표가실제로바뀌고있으므로17,192회전부중복/불필요라주장하지않는다. 미세getter최적화보다현재두계측구간중큰Update범위의실제소유자를우선해야한다.
추정: 인식공유는각송신보고서마다여러수신개체를순회해밀집시팬아웃비용이있을수있다. 하지만시야는이미SenseSeconds0.5초,플레이어목록0.2초,보고간격0.5초이다. Pursuit는이미0.1초주기·최대4계획round-robin예산을사용한다. FormationMatching은이번미션에서켜지지않아전역할당솔버를주범으로지목할근거가없다. 인식·배분·할당·GC/할당기self-time은이번로그에서분리되지않았다.
시간대분리: ID54의약66초복구루프와ID17시체대기는후반불필요작업후보이나최초전원생존30초비용을설명하지않는다. 초기부하와후반결함비용을합쳐원인으로단정하지않는다.
기존근거: 비대표rootgetter생략은이미3쌍에서개선방향불일치로미수용,4파트후보도일관된효과미입증이었다. 이미분산된Pursuit/공간격자를새로운해법으로중복제안하지않고,추가파트축소·새보간·시야전체삭제를바로시작하지않는다.
## 5. 검증 수준
진단 성공: 호출·계획·출력·반복설정후보확인. 가장큰함수의exclusive CPU시간순위는확정불가. 신규구현/성능A/B/회귀/최종수용은미실행이다.
## 6. 변경 사항
게임코드/PBO/설정수정없음. noncollision-cost-review.json/Markdown·체크포인트만추가. 원시실험기록은덮어쓰지않았다.
## 7. 원시 증거
실행ANIM-playable-sixtyfour1의gameplay-eight.tsv·combat-eight.tsv·server-frames.tsv·native-traversal-events.tsv,현행미션Commit과DGBGroundAgent/Ground/Combat/Traversal/Pursuit소스. 기존current-five-cost-coverage와follower-root-method-cost체크포인트를대조했다. 원시파일해시는기존체계에연결한다.
## 8. 잔여 문제와 우선순위
우선순위는①생존구간Update의경로/목표유효성재계획부담②320파트표시Commit과로그진단부담③사망후중복물리설정과후반복구루프다. 원시로그만으로①안의각기능CPU비중을확정하지않는다. 가장국소적인명확한중복후보는FreezeDamageReceiver의반복상태설정이지만초기전원생존문제의해결책으로과대평가하지않는다. 주력최적화를유지하고시체기능개발로확대하지않으며,다음수정은한요인만비교해야한다. 이번요청에서는수정·새시험을시작하지않았다.
